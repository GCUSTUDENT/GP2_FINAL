#pragma once

#include <glm\glm.hpp>
#include <AL\al.h>
#include <AL\alc.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <SDL\SDL.h>
#include <glm/gtc/type_ptr.hpp>

// Game_Audio class. Handles OpenAL and Vector data types
class Game_Audio
{
	// Struct for holding OpenAL data
	struct AL_Data {
		int audio_SourceID, audio_BufferID; // Integer data variables for holding unique ID identifiers for audio sources and buffers
		char* aBuffer; // Audio buffer represented by a char pointer
		std::string fileName; // The file name of the audio is stored on a string

		// AL_Data Constructor. Unsigned integer variables are given for storing OpenAL data in-function
		AL_Data(unsigned int a_SI, unsigned int a_BI, char* aB, const char* fN)
		{
			this->audio_SourceID = a_SI; // Audio source identifier assigned to unsigned integer variable 'a_SI'
			this->audio_BufferID = a_BI; // Audio buffer identifier assigned to unsigned integer variable 'a_BI'
			this->aBuffer = aB; // Audio buffer assigned to unsigned integer variable 'aB'
			this->fileName = fN; // Audio source file name assigned to unsigned integer variable 'fN'
		}
	};

	// Struct for holding Vec3 data
	struct Vec
	{
		float x, y, z; // Float variables for Vec3 x, y, z coordinates
	};

	std::vector<AL_Data> AL_DataBits; // Vector data variable 'AL_DataBits' represents data held in AL_Data struct
	ALCcontext* AL_Context; // ALCcontext data variable 'AL_Context' represents ALContext data. Pointer
	ALCdevice *AL_Device; // ALCdevice data variable 'AL_Device' represents ALCdevice data. Receiver
	bool Big_Endian(); // Boolean for enabling Big Endian data parsing functionality
	int convert_Chars_ToInt(char* alBuffer, int length); // Converts chars representing AL buffer and checks the number of char strings is above zero length 
	char* Load_WAV_File(const char* fN, int& chan, int& smplRate, int& BPS, int& size); // Loads .wav audio and establishes file name, audio channel, audio sample rate, beats per second and file size

public:
	Game_Audio(); // Game_Audio Constructor
	~Game_Audio(); // Game_Audio De-Constructor
	unsigned int Load_WAV(const char* fileName); // Loads .wav file using file name
	void Delete_WAV(unsigned int fileID); // Deletes .wav file using file identifier
	void WAV_File_ID(unsigned int fileID); // Establishes .wav file ID
	void Play_WAV(unsigned int fileID, glm::vec3& position); // Plays .wav file using file identifier at a Vec3 position
	void Stop_WAV(unsigned int fileID); // Stops .wav file using file identifier
	void Set_Listener(glm::vec3& position, glm::vec3& mainCam_LookAt); // Sets audio listener component at Vec3 position on main Camera display

private:	
};

