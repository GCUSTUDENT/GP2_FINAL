#include "Main_Game.h"
#include "Camera.h"
#include <iostream>
#include <string>

Transform transform; // Data variable 'transform' representing transform class

// Main_Game Constructor. Sets GameState, creates a new Game_Display and assigns Mesh & Game_Audio pointers
Main_Game::Main_Game()
{
	gameState = Current_GameState::PLAY; // 'gameState' is equal to current GameState 'PLAY'
	Game_Display* gameDisplay = new Game_Display(); // 'gameDisplay' is equal to a new Game_Display
    Mesh* Mesh1(); // 'Mesh1' method for accessing Mesh functions. Pointer from Mesh class
	Mesh* Mesh2(); // 'Mesh2' method for accessing Mesh functions. Pointer from Mesh class
	Mesh* Mesh3(); // 'Mesh3' method for accessing Mesh functions. Pointer from Mesh class
	Game_Audio* audioDevice(); // 'audioDevice' method for accessing Game_Audio functions. Pointer from Game_Audio class
}

// Main_Game De-Constructor
Main_Game::~Main_Game()
{
}

// runGame method, boots game application by initializing game systems and running the game loop
void Main_Game::runGame()
{
	initSys(); // Initialize game systems
	gameLoop(); // Run game loop
}

// initSys method, 
void Main_Game::initSys()
{
	gameDisplay.init_GameDisplay();  // Initialize this Game_Display
	bang = audioDevice.Load_WAV("..\\res\\bang.wav"); // Load this .wav file
	bkGround_Music = audioDevice.Load_WAV("..\\res\\background.wav"); // Load this .wav file 
	
	Mesh1.Load_Model("..\\res\\ball.obj"); // Load this .obj file
	Mesh2.Load_Model("..\\res\\ball.obj"); // Load this .obj file
	Mesh3.Load_Model("..\\res\\ball.obj"); // Load this .obj file
	
	mainCamera.Initialize_Main_Camera(glm::vec3(0, 0, -10), 70.0f, (float)gameDisplay.Get_Width()/gameDisplay.Get_Height(), 0.01f, 1000.0f); // Initialize main Camera. Set Camera position, field-of-view, aspect ratio (by accessing display Width and Height), near and far clipping planes
	setCounter = 20.0f; // Counter set to 20.0f
}

// gameLoop method, runs game loop
void Main_Game::gameLoop()
{
	while (gameState != Current_GameState::	QUIT) // While the current GameState is not equal to QUIT (the game is being played)
	{
		processInput(); // Process game inputs
		drawGame(); // Draw content in game world space
		Play_WAV_Audio(bkGround_Music, glm::vec3(0.0f,0.0f,0.0f)); // Play audio source at the Vec3 location in game world space
		collisions(Mesh1.Get_SPosition(), Mesh1.Get_SRadius(), Mesh2.Get_SPosition(), Mesh2.Get_SRadius(), Mesh3.Get_SPosition(), Mesh3.Get_SRadius()); // Check for game obj collisions by accessing Collider position and sphere radius
		
	}
}

// Processes game inputs
void Main_Game::processInput()
{
	SDL_Event event; // Data variable 'event' for checking for specific scenarios

	while(SDL_PollEvent(&event)) // Whilst events are being collected and processed
	{
		switch (event.type) // Establish event type with switch statement
		{
			case SDL_QUIT: // Case for SDL_QUIT event type
				gameState = Current_GameState::QUIT; // Data variable 'gameState' is equal to current GameState 'QUIT'
				break;
		}
	}
	
}

// Plays .wav file from an audio source at a Vec3 position in game world space
void Main_Game::Play_WAV_Audio(unsigned int audioSource, glm::vec3 position)
{
	ALint soundsState; // Integer data variable 'soundsState' representing OpenAL. Utilized as binary playing/stopped states
	alGetSourcei(audioSource, AL_SOURCE_STATE, &soundsState); // Accesses source of audio

	if (AL_PLAYING != soundsState) // If soundsState is not equal to playing
	{
		audioDevice.Play_WAV(audioSource, position); // Play audio (.wav) from audio source at Vec3 position in game world space
	}
}

// Detects game obj Collider collisions using sphere Mesh positions and radius
bool Main_Game::collisions(glm::vec3 mesh1_Pos, float mesh1_Radius, glm::vec3 mesh2_Pos, float mesh2_Radius, glm::vec3 mesh3_Pos, float mesh3_Radius)
{
	float colliderDistance = glm::sqrt((mesh2_Pos.x - mesh1_Pos.x)*(mesh2_Pos.x - mesh1_Pos.x) + (mesh2_Pos.y - mesh1_Pos.y)*(mesh2_Pos.y - mesh1_Pos.y) + (mesh2_Pos.z - mesh1_Pos.z)*(mesh2_Pos.z - mesh1_Pos.z)); // Float variable 'colliderDistance' collision checks based on the square root of mesh2_Pos minus mesh1_Pos x, y, z values
	float colliderDistance1 = glm::sqrt((mesh3_Pos.x - mesh1_Pos.x) * (mesh3_Pos.x - mesh1_Pos.x) + (mesh3_Pos.y - mesh1_Pos.y) * (mesh3_Pos.y - mesh1_Pos.y) + (mesh3_Pos.z - mesh1_Pos.z) * (mesh3_Pos.z - mesh1_Pos.z)); // Float variable 'colliderDistance1' collision checks based on the square root of mesh3_Pos minus mesh1_Pos x, y, z values
	float colliderDistance2 = glm::sqrt((mesh3_Pos.x - mesh2_Pos.x) * (mesh3_Pos.x - mesh2_Pos.x) + (mesh3_Pos.y - mesh2_Pos.y) * (mesh3_Pos.y - mesh2_Pos.y) + (mesh3_Pos.z - mesh2_Pos.z) * (mesh3_Pos.z - mesh2_Pos.z)); // Float variable 'colliderDistance2' collision checks based on the square root of mesh3_Pos minus mesh2_Pos x, y, z values

	if (colliderDistance < (mesh1_Radius + mesh2_Radius)) // If the distance between colliders is less than the Collider radius of the current and target game objs
	{
		audioDevice.Set_Listener(mainCamera.Get_Position(), mesh1_Pos); // Set audio Listener. Established at main Camera position and localised at sphere Collider position
		Play_WAV_Audio(bang, mesh1_Pos); // Play .wav file from audio source at sphere Collider position
		return true;
	}
	else
	{
		return false;
	}

	if (colliderDistance1 < (mesh1_Radius + mesh3_Radius)) // If the distance between colliders is less than the Collider radius of the current and target game objs
	{
		audioDevice.Set_Listener(mainCamera.Get_Position(), mesh1_Pos); // Set audio Listener. Established at main Camera position and localised at sphere Collider position
		Play_WAV_Audio(bang, mesh1_Pos); // Play .wav file from audio source at sphere Collider position
		return true;
	}
	else
	{
		return false;
	}

	if (colliderDistance2 < (mesh2_Radius + mesh3_Radius)) // If the distance between colliders is less than the Collider radius of the current and target game objs
	{
		audioDevice.Set_Listener(mainCamera.Get_Position(), mesh2_Pos); // Set audio Listener. Established at main Camera position and localised at sphere Collider position
		Play_WAV_Audio(bang, mesh2_Pos); // Play .wav file from audio source at sphere Collider position
		return true;
	}
	else
	{
		return false;
	}
}
// drawGame method. Controls screen functionality
void Main_Game::drawGame()
{
	gameDisplay.clear_GameDisplay(0.0f, 0.0f, 0.0f, 1.0f); // Clear this display
	
	Shader shader("..\\res\\shader"); // Data variable 'shader' for loading shader file from file path
	Textures texture("..\\res\\stripe.jpg"); // Data variable 'texture' for loading .jpg file from file path
	Textures texture1("..\\res\\spot.jpg"); // Data variable 'texture1' for loading .jpg file from file path
	Textures texture2("..\\res\\spot2.jpg"); // Data variable 'texture2' for loading .jpg file from file path
	
	transform.Set_Position(glm::vec3(sinf(setCounter), 0.5, 0.0)); // Set obj transform position values
	transform.Set_Rotation(glm::vec3(0.0, 90.0, setCounter * 1)); // Set obj transform rotation values
	transform.Set_Scale(glm::vec3(1.0, 1.0, 1.0)); // Set obj transform scale values

	shader.Bind(); // Bind shader data
	shader.Update(transform, mainCamera); // Update shader data by accessing obj transform and acquiring model being displayed on-screen
	texture.Bind(0); // Bind 'texture' data
	Mesh1.draw(); // Draw obj mesh 'Mesh1'
	Mesh1.Update_SphereCol_Data(*transform.Get_Position(), 0.62f); // Update obj's sphere Collider position and its radius
	

	transform.Set_Position(glm::vec3(-sinf(setCounter), 0.0, -sinf(setCounter)*1.5)); // Set Vec3 transform of obj
	transform.Set_Rotation(glm::vec3(0.0, 90.0, setCounter * 1)); // Set Vec3 rotation of obj
	transform.Set_Scale(glm::vec3(1.0, 1.0, 1.0)); // Set Vec3 scale of obj

	shader.Bind(); // Bind shader data
	shader.Update(transform, mainCamera); // Update shader data by accessing obj transform and acquiring model being displayed on-screen
	texture1.Bind(0); // Bind 'texture1' data
	Mesh2.draw(); // Draw obj mesh 'Mesh2'
	Mesh2.Update_SphereCol_Data(*transform.Get_Position(), 0.50f); // Update obj's sphere Collider position and its radius

	transform.Set_Position(glm::vec3(0.0, 0.25, sinf(setCounter))); // Set Vec3 transform of obj
	transform.Set_Rotation(glm::vec3(0.0, 90.0, setCounter * 1)); // Set Vec3 rotation of obj
	transform.Set_Scale(glm::vec3(1.0, 1.0, 1.0)); // Set Vec3 scale of obj

	shader.Bind(); // Bind shader data
	shader.Update(transform, mainCamera); // Update shader data by accessing obj transform and acquiring model being displayed on-screen
	texture2.Bind(0); // Bind 'texture2' data
	Mesh3.draw(); // Draw obj mesh 'Mesh3'
	Mesh3.Update_SphereCol_Data(*transform.Get_Position(), 0.62f); // Update obj's sphere Collider position and its radius
	setCounter = setCounter + 0.05f; // Counter is set to its own value plus 0.05f

				
	glEnableClientState(GL_COLOR_ARRAY); // Enables client-side OpenGL capabilities
	glEnd(); // End OpenGL operations

	gameDisplay.Buffer_Swap(); // Swap display buffer
} 