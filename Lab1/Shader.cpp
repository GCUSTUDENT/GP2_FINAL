#include "Shader.h"
#include <iostream>
#include <fstream>

// Shader Constructor. Spawns shader from file
Shader::Shader(const std::string& fileName)
{
	shaderProgram = glCreateProgram(); // Establish shader program
	shadersArray[0] = Create_Shader(Load_Shader("..\\res\\shader.vert"), GL_VERTEX_SHADER); // Establish vertex shader
	shadersArray[1] = Create_Shader(Load_Shader("..\\res\\shader.frag"), GL_FRAGMENT_SHADER); // Establish fragment shader

	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++) // Loop for every iteration of 'i' on number of shaders and incement one
	{
		glAttachShader(shaderProgram, shadersArray[i]); // Add all shaders to shader program
	}

	glBindAttribLocation(shaderProgram, 0, "position"); // Establish link between Vec3 attribute 'position' with our shader program
	glBindAttribLocation(shaderProgram, 1, "textureCoordinates"); // Establish link between Vec2 attribute 'textureCoordinates' with our shader program
	glBindAttribLocation(shaderProgram, 2, "normals"); // Establish link between Vec3 attribute 'normals' with our shader program

	glLinkProgram(shaderProgram); // Generate GPU shader executables
	Shader_Errors(shaderProgram, GL_LINK_STATUS, true, "Error: Failed to link Shader program"); // Assess shader program for errors

	glValidateProgram(shaderProgram); // Ensure shader program is valid
	Shader_Errors(shaderProgram, GL_VALIDATE_STATUS, true, "Error: Invalid Shader program"); // Assess shader program for errors

	uniforms[TRANSFORM_UNIFORM] = glGetUniformLocation(shaderProgram, "transform"); // Link 'transform' to location of uniform variable in shader program
}

// Shader De-Constructor
Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_OF_SHADERS; i++) // Loop for every iteration of 'i' in number of shaders and increment one
	{
		glDetachShader(shaderProgram, shadersArray[i]); // Detach shader from shader program
		glDeleteShader(shadersArray[i]); // Delete all shaders
	}
	glDeleteProgram(shaderProgram); // Delete shader program
}

// Bind shader program to GPU
void Shader::Bind()
{
	glUseProgram(shaderProgram); // installs the shader program for a specified object associated with rendering state
}

// Update transform and refer to main Camera viewport display
void Shader::Update(const Transform& transform, const Camera& camera)
{
	glm::mat4 mvp = camera.Get_Viewport_Projection() * transform.Get_Model(); // Mat4 variable 'mvp' equals the viewport of main Camera and the model's transform
	glUniformMatrix4fv(uniforms[TRANSFORM_UNIFORM], 1, GLU_FALSE, &mvp[0][0]); // Uniforms are set to the main camera viewport matrix
}

// Create shader using data found on strings and assign an integer variable type
GLuint Shader::Create_Shader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); // Create specified shader type

	if (shader == 0) // If shader is equal to zero then none are generated
		std::cerr << "Error: Type creation has failed to generate" << type << std::endl; // Display error

	const GLchar* charString_Source[1]; // Converts strings into a char string
	charString_Source[0] = text.c_str(); // Char string is equal to the text string converted to a char string
	GLint lengths[1]; // String length is equal to one
	lengths[0] = text.length(); // Length of amount of strings is equal to zero

	glShaderSource(shader, 1, charString_Source, lengths); // Char string source is sent to OpenGL
	glCompileShader(shader); // Let OpenGL compile shader

	Shader_Errors(shader, GL_COMPILE_STATUS, false, "Error: Cannot compile Shader"); // Assess shader for compiler errors

	return shader; // Returns shader
}

// Load shader from file name
std::string Shader::Load_Shader(const std::string& fileName)
{
	std::ifstream file; // Variable for storing and parsing shader file
	file.open((fileName).c_str()); // Open shader file at file name and convert to char string

	std::string output; // String output
	std::string line; // File line

	if (file.is_open()) // If file is currently open
	{
		while (file.good()) // While file is valid
		{
			getline(file, line); // Get file line data
			output.append(line + "\n"); // Append output of file line
		}
	}
	else
	{
		std::cerr << "Error: Cannot load shader: " << fileName << std::endl; // Display error
	}

	return output; // Returns output
}

// Displays shader errors. Identifies and locates shader which is then flagged. Shader_Errors will ensure the program is running and then display an error message 
void Shader::Shader_Errors(GLuint shader, GLuint flag, bool programRunning, const std::string& errorMsg)
{
	GLint success = 0; // Integer variable representing build success
	GLchar err[1024] = { 0 }; // Error char variable representing build errors

	if (programRunning) // If the program is currently running
		glGetProgramiv(shader, flag, &success); // Shader in program is flagged and runs successfully
	else
		glGetShaderiv(shader, flag, &success); // Shader is flagged successfully

	if (success == GL_FALSE) // If success is equal to 'GL_FALSE'
	{
		if (programRunning) // If the program is currently running
			glGetProgramInfoLog(shader, sizeof(err), NULL, err); // Shader in program is flagged as being unsuccessful on error and logged
		else
			glGetShaderInfoLog(shader, sizeof(err), NULL, err); // Shader is flagged as unsuccessful and logged

		std::cerr << errorMsg << ": '" << err << "'" << std::endl; // Display error
	}
}

