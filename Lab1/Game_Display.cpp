#include "Game_Display.h"

// Game_Display Constructor
Game_Display::Game_Display()
{
	Game_Display_SDLWindow = nullptr; // Display set to nullptr to avoid Null Access Violation 
	scrWidth = 1024.0f; // Screen Width is equal to 1024.0f
	scrHeight = 768.0f; // Screen Height is equal to 768.0f
}

// Game_Display De-Constructor
Game_Display::~Game_Display()
{
	SDL_GL_DeleteContext(GL_Context); // Delete OpenGL context
	SDL_DestroyWindow(Game_Display_SDLWindow); // Delete display
	SDL_Quit(); // Quit SDL operations
}

float Game_Display::Get_Width() { return scrWidth; } // Gets screen Width and returns it
float Game_Display::Get_Height() { return scrHeight; } // Gets screen Height and returns it

// Displays logic/compiler errors
void Game_Display::throwError(std::string errorString)
{
	std::cout << errorString << std::endl; // Output error string
	std::cout << "Press any key to QUIT."; // Display instructional 'Quit' message to User
	int in; // Expect User input
	std::cin >> in; // Accept User input
	SDL_Quit(); // Quit SDL operations
}

// Swaps display Buffer
void Game_Display::Buffer_Swap()
{
	SDL_GL_SwapWindow(Game_Display_SDLWindow); // Swap display Buffer
}

// Clears display (RGBA)
void Game_Display::clear_GameDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a); // Clear RGBA colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear display colour and depth buffer
}

// Initializes GameDisplay
void Game_Display::init_GameDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //Initialize all SDL operations

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // Minimum number of colour display bits (RED)
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); // Minimum number of colour display bits (GREEN)
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); // Minimum number of colour display bits (BLUE)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Z-buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Double buffer   

	Game_Display_SDLWindow = SDL_CreateWindow("Game Display Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)scrWidth, (int)scrHeight, SDL_WINDOW_OPENGL); // Data variable 'Game_Display_SDLWindow' creates a display

	if (Game_Display_SDLWindow == nullptr) // If data variable 'Game_Display_SDLWindow' is equal to nullptr
	{
		throwError("Failed to generate Game Display Window"); // Display error
	}

	GL_Context = SDL_GL_CreateContext(Game_Display_SDLWindow); // Data variable 'GL_Context' is equal to a new Context (display)

	if (GL_Context == nullptr) // If data variable 'GL_Context' is equal to nullptr
	{
		throwError("Failed to generate SDL_GL Context"); // Display error
	}

	GLenum error = glewInit(); // Enum variable 'error' is equal to glewInit()

	if (error != GLEW_OK) // If enum variable 'error' is not equal to GLEW_OK 
	{
		throwError("Failed to initialize GLEW"); // Display error
	}

	glEnable(GL_DEPTH_TEST); // Enable display Z-buffer
	glEnable(GL_CULL_FACE); // Cull any faces not currently being displayed by the main Camera

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f); // Clear display colour to these values
}