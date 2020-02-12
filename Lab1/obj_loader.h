#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Object_Index
{
    unsigned int vertexIndex;
    unsigned int UV_Index;
    unsigned int normalIndex;
    
    bool operator<(const Object_Index& r) const { return vertexIndex < r.vertexIndex; }
};

class Indexed_Models
{
public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    
    void Calculate_Normals();
};

class Object_Model
{
public:
    std::vector<Object_Index> Object_Indices;
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec2> UVs;
    std::vector<glm::vec3> Normals;
    bool containsUVs;
    bool containsNormals;
    
    Object_Model(const std::string& fileName);
    
    Indexed_Models To_Indexed_Model();
private:
    unsigned int Find_Previous_Vert_Index(const std::vector<Object_Index*>& index_LookUp, const Object_Index* currentIndex, const Indexed_Models& result);
    void Create_Object_Face(const std::string& line);
    
    glm::vec2 Parse_Object_Vec2(const std::string& line);
    glm::vec3 Parse_Object_Vec3(const std::string& line);
    Object_Index Parse_Object_Index(const std::string& obj_IndexToken, bool* containsUVs, bool* containsNormals);
};

#endif // OBJ_LOADER_H_INCLUDED
