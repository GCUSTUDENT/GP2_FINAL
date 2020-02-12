#include "Textures.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

//  Textures Constructor. Loads texture from file, analyses texture data and throws an error if there is an error loading the texture 
Textures::Textures(const std::string& fileName)
{
	int width, height, numOf_Components; // Width, Height and number of components of texture image
	unsigned char* imgData = stbi_load((fileName).c_str(), &width, &height, &numOf_Components, 4); // Load the texture image from file and store the data

	if (imgData == NULL) // If 'imgData' is equal to NULL
	{
		std::cerr << "Error: Failed to load Texture" << fileName << std::endl; // Display error
	}

	glGenTextures(1, &texturesHandler); // Number of textures and their addresses
	glBindTexture(GL_TEXTURE_2D, texturesHandler); // Define texture type and bind texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrap texture on the outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // wrap texture on the outside height

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear filter applied to texture for minification, makes texture smaller than surface area
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear filter applied to texture for magnification, makes texture larger than surface area

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData); //Target texture, mipmapping level set to zero, pixel format is RGBA, Width set, Height set, border size set to zero, input format set to RGBA, data type of texture set to unsigned_byte and image data set

	stbi_image_free(imgData); // Image data sent to stb_image
}

// Textures de-Constructor
Textures::~Textures()
{
	glDeleteTextures(1, &texturesHandler); // Delete number of textures and their addresses
}

// Binds texture
void Textures::Bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31); /// Assert that program is using one of thirty-two textures

	glActiveTexture(GL_TEXTURE0 + unit); // Set the active texture unit
	glBindTexture(GL_TEXTURE_2D, texturesHandler); // Define texture type and bind texture to unit
}
