#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;

// Game_Display class
class Game_Display
{
public:
	Game_Display(); // Game_Display Constructor
	~Game_Display(); // Game_Display De-Constructor
	void init_GameDisplay(); // Initializes GameDisplay
	void Buffer_Swap(); // Swaps display Buffer
	void clear_GameDisplay(float r, float g, float b, float a); // Clear GameDisplay (RGBA format)

	float Get_Width(); // Get screen Width
	float Get_Height(); // Get screen Height

private:

	// Brings up an error on a dedicated error string when there is a logic/compiler problem
	void throwError(std::string errorString);
	
	SDL_GLContext GL_Context; // Global data variable 'GL_Context'. Represents SDL's OpenGL Context
	SDL_Window* Game_Display_SDLWindow; // Global data variable 'Game_Display_SDLWindow'. Represents SDL display Window
	float scrWidth; // Float variable for screen Width
	float scrHeight; // Float variable for screen Height
};

