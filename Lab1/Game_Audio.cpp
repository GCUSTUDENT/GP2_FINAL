#include "Game_Audio.h"

// Game_Audio Constructor
Game_Audio::Game_Audio()
{
	AL_Device = alcOpenDevice(NULL); // 'AL_Device' variable opens sound card component

	if (AL_Device == NULL) // If 'AL_Device' is equal to NULL
	{
		std::cout << "Error: Failed to access Sound settings/Sound card" << std::endl; // Output error
	}

	AL_Context = alcCreateContext(AL_Device, NULL); // 'AL_Context' generates ALCcontext on device

	if (AL_Context == NULL) // If 'AL_Context' is equal to NULL
	{
		std::cout << "Error: Failed to load AL Context" << std::endl; // Output error
	}

	alcMakeContextCurrent(AL_Context); // Make the current ALCcontext equal to 'AL_Context'
}

// Game_Audio De-Constructor
Game_Audio::~Game_Audio()
{
	for (unsigned int i = 0; i < AL_DataBits.size(); i++) // Loop for every iteration of 'i' in 'AL_DataBits' and increment by one
	{
		alDeleteSources(1, (unsigned int*)&AL_DataBits[i].audio_SourceID); // Delete all AL_Data bits from audio source

		if (AL_DataBits[i].audio_BufferID != -1) // if array of AL_Data bits for OpenAL buffer is not equal to minus one
		{
			alDeleteBuffers(1, (unsigned int*)&AL_DataBits[i].audio_BufferID); // Delete all AL_Data bits from audio buffer
			delete[] AL_DataBits[i].aBuffer; // Delete OpenAL buffer
		}
	}
	alcDestroyContext(AL_Context); // Destroy OpenAL context
	alcCloseDevice(AL_Device); // Close device
}

// Sets Game_Audio data on integer variable
bool Game_Audio::Big_Endian()
{
	int aud = 1; // Integer variable 'aud' (represents audio data) equal to one
	return !((char*)&aud)[0]; // Do not return any chars with zero audio data
}

// Converts chars storing OpenAL buffer data to integer variable (aud) when number of char strings is above zero
int Game_Audio::convert_Chars_ToInt(char* alBuffer, int length)
{
	int aud = 0; // Integer variable 'aud' is equal to zero

	if (!Big_Endian()) // if Big_Endian() is false

		for (int i = 0; i < length; i++) // Loop for each iteration of 'i' on length of char strings
			((char*)&aud)[i] = alBuffer[i]; // Integer variable 'aud' is equal to number of char strings on OpenAL buffer array [i]
	else
		for (int i = 0; i < length; i++) // Loop for each iteration of 'i' on length of char strings
			((char*)&aud)[3 - i] = alBuffer[i]; // Integer variable 'aud' is equal to three minus number of char strings on OpenAL buffer array [i]
	
	return aud; // Returns integer variable 'aud'
}

// Load .wav file file name, audio channel, sample rate, beats per second and file size
char* Game_Audio::Load_WAV_File(const char* fN, int& channel, int& smplRate, int& BPS, int& size)
{
	char AL_Buffer[4]; // 'AL_Buffer' char size set to four
	std::ifstream in(fN, std::ios::binary); // Take in filename

	in.read(AL_Buffer, 4); // Read 'AL_Buffer' data being taken in
	in.read(AL_Buffer, 4); // Read 'AL_Buffer' data being taken in
	in.read(AL_Buffer, 4); // Read 'AL_Buffer' data being taken in
	in.read(AL_Buffer, 4); // Read 'AL_Buffer' data being taken in
	in.read(AL_Buffer, 4); // Read 'AL_Buffer' data being taken in
	in.read(AL_Buffer, 2); // Read 'AL_Buffer' data being taken in
	in.read(AL_Buffer, 2); // Read 'AL_Buffer' data being taken in

	channel = convert_Chars_ToInt(AL_Buffer, 2); // Integer variable 'channel' char-to-integer conversion

	in.read(AL_Buffer, 4); // Read 'AL_Buffer' data being taken in

	smplRate = convert_Chars_ToInt(AL_Buffer, 4); // Integer variable 'smplrate' char-to-integer conversion

	in.read(AL_Buffer, 4); // Read 'AL_Buffer' data being taken in
	in.read(AL_Buffer, 2); // Read 'AL_Buffer' data being taken in
	in.read(AL_Buffer, 2); // Read 'AL_Buffer' data being taken in

	BPS = convert_Chars_ToInt(AL_Buffer, 2); // Integer variable 'BPS' char-to-integer conversion

	in.read(AL_Buffer, 4); // Read 'AL_Buffer' data being taken in
	in.read(AL_Buffer, 4); // Read 'AL_Buffer' data being taken in

	size = convert_Chars_ToInt(AL_Buffer, 4); // Integer variable 'BPS' char-to-integer conversion

	char* audioData = new char[size]; // Char 'audioData' is equal to a new char size
	in.read(audioData, size); // Read 'audioData' and its analyse its size

	return audioData; // Returns char 'audioData'
}

// Loads .wav audio data from file name
unsigned int Game_Audio::Load_WAV(const char* fileName)
{
	bool foundWAV = false; // Boolean variable 'foundWAV' is equal to false
	unsigned int audio_SourceID, audio_BufferID; // Two unsigned integer variables 'audio_SourceID' and 'audio_BufferID for storing audio source/buffer data
	char* audioData = NULL; // Char 'audioData' equal to NULL. Pointer

	for (unsigned int i = 0; i < AL_DataBits.size(); i++) // Loop for every iteration of 'i' on size of Vector data variable 'AL_DataBits'
	{
		if (AL_DataBits[i].fileName == fileName && AL_DataBits[i].audio_BufferID != -1) // If AL_Data bit has .wav file name and 'audio_BufferID' is not equal to minus one
		{
			audio_BufferID = AL_DataBits[i].audio_BufferID; // Unsigned int 'audio_BufferID' is equal to AL_Data bit and is added to array
			foundWAV = true; // Boolean variable 'foundWAV' is equal to true
			break;
		}
	}

	if(!foundWAV) // if boolean variable 'foundWAV' is false
		{	
			int channel, smplRate, BPS, size; // Establish integer variables 'channel', 'smplRate', 'BPS' and 'size'
			audioData = Load_WAV_File(fileName, channel, smplRate, BPS, size); // Char 'audioData' loads .wav file name, audio channel, sample rate, beats per second and file size
			unsigned int audio_FileFormat; // Unsigned integer variable 'audio_FileFormat'
			alGenBuffers(1, &audio_BufferID); // Generate OpenAL buffer 'audio_BufferID'

			if (channel == 1) // If audio channel is equal to one
			{
				if (BPS == 8) // If beats per second is equal to eight
				{
					audio_FileFormat = AL_FORMAT_MONO8; // Audio format is equal to 'AL_FORMAT_MONO8'
				}
				else
				{
					audio_FileFormat = AL_FORMAT_MONO16; // Audio format is equal to 'AL_FORMAT_MONO16'
				}
			}
			else
			{
				if (BPS == 8) // If beats per second is equal to eight
				{
					audio_FileFormat = AL_FORMAT_STEREO8; // Audio format is equal to 'AL_FORMAT_STEREO8'
				}
				else
				{
					audio_FileFormat = AL_FORMAT_STEREO16; // Audio format is equal to 'AL_FORMAT_STEREO16'
				}

			}

			alBufferData(audio_BufferID, audio_FileFormat, audioData, size, smplRate); // OpenAL buffer data
		}

	alGenSources(1, &audio_SourceID); // Generate OpenAL audio sources

	alSourcei(audio_SourceID, AL_BUFFER, audio_BufferID); // Audio source 'i'
	alSourcef(audio_SourceID, AL_REFERENCE_DISTANCE, 1.0f); // Audio source 'f'

	AL_DataBits.push_back(AL_Data(audio_SourceID, (!foundWAV ? audio_BufferID : -1), audioData, fileName)); // Push AL_Data bits if a .wav has not been found and 'audio_BufferID' is equal to minus one
	
	return audio_SourceID; // Returns identity of audio source
}

// Deletes .wav audio file identity
void Game_Audio::Delete_WAV(unsigned int fileID) 
{}

// Accesses .wav audio from file identity
void Game_Audio::WAV_File_ID(unsigned int fileID) 
{
	alSourcePlay(fileID); // Play .wav file
}

// Play .wav audio from file identity at a Vec3 position
void Game_Audio::Play_WAV(unsigned int fileID, glm::vec3& position) 
{
	alSource3f(fileID, AL_POSITION, position.x, position.y, position.z); // Plays .wav file at x, y, z coordinates in game world space
	alSourcePlay(fileID); // Play .wav file
}

//  Stops .wav audio from file identity
void Game_Audio::Stop_WAV(unsigned int fileID) 
{
	alSourceStop(fileID); // Stop .wav file
}

// Sets audio location in game world space using a Vec3 position on the main Camera display
void Game_Audio::Set_Listener(glm::vec3& position, glm::vec3& mainCam_LookAt) 
{
	alListener3f(AL_POSITION, position.x, position.y, -position.z); // Set OpenAL audio listener at these x, y, z coordinates
	float orient[6] = { mainCam_LookAt.x,mainCam_LookAt.y,mainCam_LookAt.z, 0, 1, 0 }; // Orient sound origin at these x, y, z coordinates on main Camera display
	alListenerfv(AL_ORIENTATION, orient); // OpenAL audio listener transform float values for orientation are equal to  float variable data 'orient'
}