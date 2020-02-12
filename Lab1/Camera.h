#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Main camera struct for use in Main_Game.cpp
struct Camera
{
public:

	// Main camera constructor
	Camera() 
	{
	}

	// Initializes Main camera components
	void Initialize_Main_Camera(const glm::vec3& pos, float fov, float aspectRatio, float nearClip, float farClip)
	{
		this->pos = pos; // Main camera position
		this->fwd = glm::vec3(0.0f, 0.0f, 1.0f); // Main camera forward vector
		this->up = glm::vec3(0.0f, 1.0f, 0.0f); // Main camera up vector
		this->projection = glm::perspective(fov, aspectRatio, nearClip, farClip); // Main camera projection (field-of-view, aspect ratio, near and far Clipping planes
	}

	// Get Main camera position
	glm::vec3 Get_Position()
	{
		return this->pos;
	}

	// Get Main camera projection
	inline glm::mat4 Get_Viewport_Projection() const
	{
		return projection * glm::lookAt(pos, pos + fwd, up);
	}

protected:
private:
	glm::mat4 projection; // Mat4 projection data
	glm::vec3 pos; // Vec3 position data
	glm::vec3 fwd; // Vec3 foward data
	glm::vec3 up; // Vec3 up data
};


