#pragma once
#include <string>
#include <GL\glew.h>

// Textures class. Binds textures
class Textures
{
public:
	//Textures Constructor
	Textures(const std::string& fileName); 

	// Bind textures
	void Bind(unsigned int unit);

	// Textures De-Constructor
	~Textures();

protected:
private:

	GLuint texturesHandler; // GLuint variable 'texturesHandler' for storing and parsing texture data
};

