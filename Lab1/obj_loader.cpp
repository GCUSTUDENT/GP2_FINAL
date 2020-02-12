#include "obj_loader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool Compare_Object_Index_Param(const Object_Index* a, const Object_Index* b);
static inline unsigned int Find_Next_Char(unsigned int start, const char* string, unsigned int length, char char_IndexToken);
static inline unsigned int Parse_Index_Val(const std::string& val_IndexToken, unsigned int start, unsigned int end);
static inline float Parse_Obj_Float_Val(const std::string& fVal_IndexToken, unsigned int start, unsigned int end);
static inline std::vector<std::string> Split_Strings(const std::string &str, char delim);

Object_Model::Object_Model(const std::string& fileName)
{
	containsUVs = false;
	containsNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            unsigned int lengthOf_Line = line.length();
            
            if(lengthOf_Line < 2)
                continue;
            
            const char* charStr = line.c_str();
            
            switch(charStr[0])
            {
                case 'v':
                    if(charStr[1] == 't')
                        this->UVs.push_back(Parse_Object_Vec2(line));
                    else if(charStr[1] == 'n')
                        this->Normals.push_back(Parse_Object_Vec3(line));
                    else if(charStr[1] == ' ' || charStr[1] == '\t')
                        this->Vertices.push_back(Parse_Object_Vec3(line));
                break;
                case 'f':
                    Create_Object_Face(line);
                break;
                default: break;
            };
        }
    }
    else
    {
        std::cerr << "Loading MESH failed. Filename: " << fileName << std::endl;
    }
}

void Indexed_Models::Calculate_Normals()
{
    for(unsigned int i = 0; i < indices.size(); i += 3)
    {
        int indices0 = indices[i];
        int indices1 = indices[i + 1];
        int indices2 = indices[i + 2];

        glm::vec3 vec1 = positions[indices1] - positions[indices0];
        glm::vec3 vec2 = positions[indices2] - positions[indices0];
        
        glm::vec3 normal = glm::normalize(glm::cross(vec1, vec2));
            
        normals[indices0] += normal;
        normals[indices1] += normal;
        normals[indices2] += normal;
    }
    
    for(unsigned int i = 0; i < positions.size(); i++)
        normals[i] = glm::normalize(normals[i]);
}

Indexed_Models Object_Model::To_Indexed_Model()
{
    Indexed_Models result;
    Indexed_Models normalModel;
    
    unsigned int numOf_Indices = Object_Indices.size();
    
    std::vector<Object_Index*> index_LookUp;
    
    for(unsigned int i = 0; i < numOf_Indices; i++)
        index_LookUp.push_back(&Object_Indices[i]);
    
    std::sort(index_LookUp.begin(), index_LookUp.end(), Compare_Object_Index_Param);
    
    std::map<Object_Index, unsigned int> nModel_IndexMap;
    std::map<unsigned int, unsigned int> indexMap;
    
    for(unsigned int i = 0; i < numOf_Indices; i++)
    {
        Object_Index* currentIndex = &Object_Indices[i];
        
        glm::vec3 currentPos = Vertices[currentIndex->vertexIndex];
        glm::vec2 current_TextureCoordinates;
        glm::vec3 currentNormal;
        
        if(containsUVs)
            current_TextureCoordinates = UVs[currentIndex->UV_Index];
        else
            current_TextureCoordinates = glm::vec2(0,0);
            
        if(containsNormals)
            currentNormal = Normals[currentIndex->normalIndex];
        else
            currentNormal = glm::vec3(0,0,0);
        
        unsigned int nModel_Index;
        unsigned int rModel_Index;
        
        //Create model to properly generate normals on
        std::map<Object_Index, unsigned int>::iterator iterate = nModel_IndexMap.find(*currentIndex);
        if(iterate == nModel_IndexMap.end())
        {
            nModel_Index = normalModel.positions.size();
        
            nModel_IndexMap.insert(std::pair<Object_Index, unsigned int>(*currentIndex, nModel_Index));
            normalModel.positions.push_back(currentPos);
            normalModel.textureCoordinates.push_back(current_TextureCoordinates);
            normalModel.normals.push_back(currentNormal);
        }
        else
            nModel_Index = iterate->second;
        
        //Create model which properly separates texture coordinates
        unsigned int previous_VertexLoc = Find_Previous_Vert_Index(index_LookUp, currentIndex, result);
        
        if(previous_VertexLoc == (unsigned int)-1)
        {
            rModel_Index = result.positions.size();
        
            result.positions.push_back(currentPos);
            result.textureCoordinates.push_back(current_TextureCoordinates);
            result.normals.push_back(currentNormal);
        }
        else
            rModel_Index = previous_VertexLoc;
        
        normalModel.indices.push_back(nModel_Index);
        result.indices.push_back(rModel_Index);
        indexMap.insert(std::pair<unsigned int, unsigned int>(rModel_Index, nModel_Index));
    }
    
    if(!containsNormals)
    {
        normalModel.Calculate_Normals();
        
        for(unsigned int i = 0; i < result.positions.size(); i++)
            result.normals[i] = normalModel.normals[indexMap[i]];
    }
    
    return result;
};

unsigned int Object_Model::Find_Previous_Vert_Index(const std::vector<Object_Index*>& index_LookUp, const Object_Index* currentIndex, const Indexed_Models& result)
{
    unsigned int start = 0;
    unsigned int end = index_LookUp.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;
    
    while(current != previous)
    {
        Object_Index* indexTests = index_LookUp[current];
        
        if(indexTests->vertexIndex == currentIndex->vertexIndex)
        {
            unsigned int total_StartCount = current;
        
            for(unsigned int i = 0; i < current; i++)
            {
                Object_Index* possibleIndexes = index_LookUp[current - i];
                
                if(possibleIndexes == currentIndex)
                    continue;
                    
                if(possibleIndexes->vertexIndex != currentIndex->vertexIndex)
                    break;
                    
                total_StartCount--;
            }
            
            for(unsigned int i = total_StartCount; i < index_LookUp.size() - total_StartCount; i++)
            {
                Object_Index* possibleIndexes = index_LookUp[current + i];
                
                if(possibleIndexes == currentIndex)
                    continue;
                    
                if(possibleIndexes->vertexIndex != currentIndex->vertexIndex)
                    break;
                else if((!containsUVs || possibleIndexes->UV_Index == currentIndex->UV_Index) 
                    && (!containsNormals || possibleIndexes->normalIndex == currentIndex->normalIndex))
                {
                    glm::vec3 currentPos = Vertices[currentIndex->vertexIndex];
                    glm::vec2 current_TextureCoordinates;
                    glm::vec3 currentNormal;
                    
                    if(containsUVs)
                        current_TextureCoordinates = UVs[currentIndex->UV_Index];
                    else
                        current_TextureCoordinates = glm::vec2(0,0);
                        
                    if(containsNormals)
                        currentNormal = Normals[currentIndex->normalIndex];
                    else
                        currentNormal = glm::vec3(0,0,0);
                    
                    for(unsigned int j = 0; j < result.positions.size(); j++)
                    {
                        if(currentPos == result.positions[j] 
                            && ((!containsUVs || current_TextureCoordinates == result.textureCoordinates[j])
                            && (!containsNormals || currentNormal == result.normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return -1;
        }
        else
        {
            if(indexTests->vertexIndex < currentIndex->vertexIndex)
                start = current;
            else
                end = current;
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return -1;
}

void Object_Model::Create_Object_Face(const std::string& line)
{
    std::vector<std::string> tokens = Split_Strings(line, ' ');

    this->Object_Indices.push_back(Parse_Object_Index(tokens[1], &this->containsUVs, &this->containsNormals));
    this->Object_Indices.push_back(Parse_Object_Index(tokens[2], &this->containsUVs, &this->containsNormals));
    this->Object_Indices.push_back(Parse_Object_Index(tokens[3], &this->containsUVs, &this->containsNormals));

    if((int)tokens.size() > 4)
    {
        this->Object_Indices.push_back(Parse_Object_Index(tokens[1], &this->containsUVs, &this->containsNormals));
        this->Object_Indices.push_back(Parse_Object_Index(tokens[3], &this->containsUVs, &this->containsNormals));
        this->Object_Indices.push_back(Parse_Object_Index(tokens[4], &this->containsUVs, &this->containsNormals));
    }
}

Object_Index Object_Model::Parse_Object_Index(const std::string& obj_IndexToken, bool* containsUVs, bool* containsNormals)
{
    unsigned int obj_TokenLength = obj_IndexToken.length();
	const char* tokenString = obj_IndexToken.c_str();
    
    unsigned int start_VertIndex = 0;
    unsigned int end_VertIndex = Find_Next_Char(start_VertIndex, tokenString, obj_TokenLength, '/');
    
    Object_Index result;
    result.vertexIndex = Parse_Index_Val(obj_IndexToken, start_VertIndex, end_VertIndex);
    result.UV_Index = 0;
    result.normalIndex = 0;
    
    if(end_VertIndex >= obj_TokenLength)
        return result;
    
    start_VertIndex = end_VertIndex + 1;
    end_VertIndex = Find_Next_Char(start_VertIndex, tokenString, obj_TokenLength, '/');
    
    result.UV_Index = Parse_Index_Val(obj_IndexToken, start_VertIndex, end_VertIndex);
    *containsUVs = true;
    
    if(end_VertIndex >= obj_TokenLength)
        return result;
    
    start_VertIndex = end_VertIndex + 1;
    end_VertIndex = Find_Next_Char(start_VertIndex, tokenString, obj_TokenLength, '/');
    
    result.normalIndex = Parse_Index_Val(obj_IndexToken, start_VertIndex, end_VertIndex);
    *containsNormals = true;
    
    return result;
}

glm::vec3 Object_Model::Parse_Object_Vec3(const std::string& line) 
{
    unsigned int obj_TokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int start_VertIndex = 2;
    
    while(start_VertIndex < obj_TokenLength)
    {
        if(tokenString[start_VertIndex] != ' ')
            break;
        start_VertIndex++;
    }
    
    unsigned int end_VertIndex = Find_Next_Char(start_VertIndex, tokenString, obj_TokenLength, ' ');
    
    float x = Parse_Obj_Float_Val(line, start_VertIndex, end_VertIndex);
    
    start_VertIndex = end_VertIndex + 1;
    end_VertIndex = Find_Next_Char(start_VertIndex, tokenString, obj_TokenLength, ' ');
    
    float y = Parse_Obj_Float_Val(line, start_VertIndex, end_VertIndex);
    
    start_VertIndex = end_VertIndex + 1;
    end_VertIndex = Find_Next_Char(start_VertIndex, tokenString, obj_TokenLength, ' ');
    
    float z = Parse_Obj_Float_Val(line, start_VertIndex, end_VertIndex);
    
    return glm::vec3(x,y,z);

    //glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

glm::vec2 Object_Model::Parse_Object_Vec2(const std::string& line)
{
    unsigned int obj_TokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int start_VertIndex = 3;
    
    while(start_VertIndex < obj_TokenLength)
    {
        if(tokenString[start_VertIndex] != ' ')
            break;
        start_VertIndex++;
    }
    
    unsigned int end_VertIndex = Find_Next_Char(start_VertIndex, tokenString, obj_TokenLength, ' ');
    
    float x = Parse_Obj_Float_Val(line, start_VertIndex, end_VertIndex);
    
    start_VertIndex = end_VertIndex + 1;
    end_VertIndex = Find_Next_Char(start_VertIndex, tokenString, obj_TokenLength, ' ');
    
	float y = Parse_Obj_Float_Val(line, start_VertIndex, end_VertIndex);
    
    return glm::vec2(x,y);
}

static bool Compare_Object_Index_Param(const Object_Index* a, const Object_Index* b)
{
    return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int Find_Next_Char(unsigned int start, const char* string, unsigned int length, char char_IndexToken)
{
    unsigned int result = start;
    while(result < length)
    {
        result++;
        if(string[result] == char_IndexToken)
            break;
    }
    
    return result;
}

static inline unsigned int Parse_Index_Val(const std::string& val_IndexToken, unsigned int start, unsigned int end)
{
    return atoi(val_IndexToken.substr(start, end - start).c_str()) - 1;
}

static inline float Parse_Obj_Float_Val(const std::string& fVal_IndexToken, unsigned int start, unsigned int end)
{
    return atof(fVal_IndexToken.substr(start, end - start).c_str());
}

static inline std::vector<std::string> Split_Strings(const std::string &str, char delim)
{
    std::vector<std::string> elements;
        
    const char* cstr = str.c_str();
    unsigned int stringLength = str.length();
    unsigned int start_OfString = 0;
    unsigned int end_OfString = 0;
        
	while(end_OfString <= stringLength)
    {
        while(end_OfString <= stringLength)
        {
            if(cstr[end_OfString] == delim)
                break;
            end_OfString++;
        }
            
        elements.push_back(str.substr(start_OfString, end_OfString - start_OfString));
        start_OfString = end_OfString + 1;
        end_OfString = start_OfString;
    }
        
    return elements;
}
