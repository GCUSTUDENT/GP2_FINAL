#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "obj_loader.h"

// Vertex struct for storing, getting and setting Vec3 position, Vec2 texture coordinates and Vec3 mesh normals
struct Vertex
{
public:

	// Vertex Constructor
	Vertex(const glm::vec3& position, const glm::vec2& textureCoordinates)
	{
		this->position = position; // This 'position' variable is equal to Vec3 'position'
		this->textureCoordinates = textureCoordinates; // This 'textureCoordinates' variable is equal to Vec2 'textureCoordinates'
		this->normal = normal; // This 'normal' variable is equal to Vec3 'normal'
	}

	glm::vec3* Get_Position() { return &position; } // Gets Vec3 position and returns it. Pointer
	glm::vec2* Get_Texture_Coordinates() { return &textureCoordinates; } // Gets Vec2 texture coordinates and returns it. Pointer
	glm::vec3* Get_Normal() { return &normal; } // Gets Vec3 normal and returns it. Pointer

private:
	glm::vec3 position; // Vec3 position data
	glm::vec2 textureCoordinates; // Vec2 texture coordinates data
	glm::vec3 normal; // Vec3 normal data
};

// Sphere struct for storing, getting and setting mesh Collider sphere radius
struct Sphere
{
public:

	// Sphere Constructor
	Sphere() {}

	// Sphere position and radius size
	Sphere(glm::vec3& position, float sphereRadius)
	{
		this->position = position; // This 'position' variable is equal to Vec3 'position'
	}

	glm::vec3 Get_Position() { return position; } // Gets sphere Vec3 position and returns it
	float Get_Sphere_Radius() { return sphereRadius; } // Gets sphere Vec3 position and returns it

	// Sets sphere Vec3 position
	void Set_Position(glm::vec3 position)
	{
		this->position = position; // This 'position' variable is equal to Vec3 'position'
	}

	// Sets sphere Vec3 position
	void Set_Sphere_Radius(float sphereRadius)
	{
		this->sphereRadius = sphereRadius; // This 'position' variable is equal to Vec3 'position'
	}

private:
	glm::vec3 position; // Vec3 position data
	float sphereRadius; // Vec3 sphere radius data
};

// Mesh class, sets up all obj mesh components
class Mesh
{
public:
	Mesh(); // Mesh Constructor
	~Mesh(); // Mesh De-Constructor


	void draw(); // Draws meshes
	void Initialize(Vertex* Verts, unsigned int numOf_Vertices, unsigned int* indices, unsigned int numOf_Indices); // Initializes mesh vertices and indices + amount of both vertices/indices
	void Load_Model(const std::string& filename); // Load mesh model from file name
	void Initialize_Model(const Indexed_Models& model); // Initializes mesh model from index list of mesh models
	void Update_SphereCol_Data(glm::vec3 position, float sphereRadius); // Update mesh sphere Collider data (Vec3 position and sphere radius)
	glm::vec3 Get_SPosition() { return meshSphere.Get_Position(); } // Gets Vec3 position of sphere Collider and return it
	float Get_SRadius() { return meshSphere.Get_Sphere_Radius(); } // Gets float value of sphere Collider radius and return it

private:


	// Enum for parsing different buffer data types
	enum
	{
		POSITION_VERTEX_BUFFER, // Position vertex buffer type
		TEXTURECOORDINATES_VERT_BUFFER, // Texture coordinates vertex buffer type
		NORMAL_VERT_BUFFER, // Normal vertex buffer type
		INDEX_VERT_BUFFER, // Index vertex buffer type
		NUM_OF_BUFFERS // Number of total buffers
	};

	Sphere meshSphere; // Data variable 'meshSphere' representing Sphere struct
	GLuint vertex_ArrayObject; // Data variable 'vertex_ArrayObject' creates obj vertex for buffer array
	GLuint vertex_ArrayBuffers[NUM_OF_BUFFERS]; // Data variable 'vertex_ArrayBuffers' creates buffer array
	unsigned int draw_Count; // Number of vertex_ArrayObject being drawn
};