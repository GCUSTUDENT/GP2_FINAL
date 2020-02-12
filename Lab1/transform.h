#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "camera.h"

// Transform struct for use in Main_Game.cpp
struct Transform
{
public:

	// Transform (position, rotation and scale) constructor
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rotation = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->pos = pos; // This 'pos' is equal to pos vec3
		this->rotation = rotation; // This 'rotation' is equal to rotation vec3
		this->scale = scale; // This 'scale' is equal to scale vec3
	}

	// Inline for acquiring OBJ model(s) transform position, rotation and scale 
	inline glm::mat4 Get_Model() const
	{
		glm::mat4 materialPos = glm::translate(pos); // Mat4 materialPos data variable equals 'pos' (material position translated via glm::translate)
		glm::mat4 materialScale = glm::scale(scale); // Mat4 materialScale data variable equals 'scale' (material scale size controlled via glm::scale)
		
		glm::mat4 rotX = glm::rotate(rotation.x, glm::vec3(1.0, 0.0, 0.0)); // Mat4 rotX data variable equals 'rotation' (x axis rotation controlled via glm::vec3) 
		glm::mat4 rotY = glm::rotate(rotation.y, glm::vec3(0.0, 1.0, 0.0)); // Mat4 rotY data variable equals 'rotation' (y axis rotation controlled via glm::vec3)
		glm::mat4 rotZ = glm::rotate(rotation.z, glm::vec3(0.0, 0.0, 1.0)); // Mat4 rotZ data variable equals 'rotation' (z axis rotation controlled via glm::vec3)
		
		glm::mat4 materialRotation = rotX * rotY * rotZ; // Mat4 materialRotation is equal to 'rotX' times 'rotY' times 'rotZ' values

		return materialPos * materialRotation * materialScale; // Return 'materialPos' times 'materialRotation' times 'materialScale' values
	}

	inline glm::vec3* Get_Position() { return &pos; } // Position getter function. Returns 'pos' data variable
	inline glm::vec3* Get_Rotation() { return &rotation; } // Rotation getter function. Returns 'rotation' data variable
	inline glm::vec3* Get_Scale() { return &scale; } // Scale getter function. Returns 'scale' data variable

	inline void Set_Position(glm::vec3& pos) { this->pos = pos; } // Position setter function. Sets vec3 'pos' data variable
	inline void Set_Rotation(glm::vec3& rotation) { this->rotation = rotation; } // Rotation setter function. Sets vec3 'rotation' data variable
	inline void Set_Scale(glm::vec3& scale) { this->scale = scale; } // Scale setter function. Sets vec3 'scale' data variable

protected:
private:
	glm::vec3 pos; // Vec3 position data
	glm::vec3 rotation; // Vec3 rotation data
	glm::vec3 scale; // Vec3 scale data
};


