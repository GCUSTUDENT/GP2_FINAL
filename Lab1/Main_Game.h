#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Game_Display.h"
#include "Game_Audio.h"
#include "Shader.h"
#include "Mesh.h"
#include "Textures.h"
#include "transform.h"

// Main_Game.h enum class Current_GameState. For cycling between play states playing and NOT playing
enum class Current_GameState{PLAY, QUIT};

// Main_Game class
class Main_Game
{
public:
	Main_Game(); // Main_Game Constructor
	~Main_Game(); // Main_Game De-Constructor

	void runGame(); // runGame method, boots game application

private:

	void initSys(); // Method for initializing game systems
	void processInput(); // Method for processing game inputs
	void gameLoop(); // Method for running game loop
	void drawGame(); // Method for drawing game
	bool collisions(glm::vec3 mesh1_Pos, float mesh1_Radius, glm::vec3 mesh2_Pos, float mesh2_Radius, glm::vec3 mesh3_Pos, float mesh3_Radius); // Method for object collision (world space position and radius size)
	void Play_WAV_Audio(unsigned int audioSource, glm::vec3 position); // Method that plays audio at a position in game world space

	Game_Display gameDisplay; // Data variable 'gameDisplay' representing Game_Display class
	Current_GameState gameState; // Data variable 'gameState' representing Main_Game enum class
	Mesh Mesh1; // Data variable 'Mesh1' representing Mesh class
	Mesh Mesh2; // Data variable 'Mesh2' representing Mesh class
	Mesh Mesh3; // Data variable 'Mesh3' representing Mesh class
	Game_Audio audioDevice; // Data variable 'audioDevice' representing Game_Audio class
	Camera mainCamera; // Data variable 'mainCamera' representing Camera struct

	float setCounter; // Counter float variable 'setCounter' is used in Mesh positions/rotations/scales
	unsigned int bang; // Unsigned integer variable 'whistle' is used in Game_Audio positions/sources
	unsigned int bkGround_Music; // Unsigned integer variable 'bkGround_Music' is used in Game_Audio positions/sources
};

