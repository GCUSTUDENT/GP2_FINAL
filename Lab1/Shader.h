#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "Camera.h"

// Shader class. Creates, loads, binds and updates shaders
class Shader
{
public:
	Shader(const std::string& fileName); // Shader Constructor. Takes name of shader file

	void Bind(); // Bind shader program to GPU
	void Update(const Transform& transform, const Camera& mainCamera); // Update shader transform and reference that component on main Camera display

	std::string Shader::Load_Shader(const std::string& fileName); // Loads shader from file name
	
	GLuint Shader::Create_Shader(const std::string& text, unsigned int type); // Creates shader from string and assigns integer variable type

	void Shader::Shader_Errors(GLuint shader, GLuint flag, bool programRunning, const std::string& errorMsg); // Displays shader errors (by identifying shader, flagging the shader with correct error code, check the program is currently running and store the error message on a string)

    ~Shader(); // Shader De-Constructor.


protected:
private:
	static const unsigned int NUM_OF_SHADERS = 2; // Static and const unsigned integer variable that declares the number of shaders being used

	// Enum for parsing obj uniform data with variables
	enum
	{
		TRANSFORM_UNIFORM, // Transform uniform

		NUM_OF_UNIFORMS // The number of transform uniforms
	};

	GLuint shaderProgram; // GLUint variable 'shaderProgram' tracks program shader functions
	GLuint shadersArray[NUM_OF_SHADERS]; // Number of shaders on array
	GLuint uniforms[NUM_OF_UNIFORMS]; // Number of uniforms on array
};
