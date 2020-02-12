#include "Mesh.h"
#include <vector>

// Initialize vertices/indices and the total number of both components
void Mesh::Initialize(Vertex* Verts, unsigned int numOf_Vertices, unsigned int* indices, unsigned int numOf_Indices)
{
	Indexed_Models model; // Data variable 'model' represents 'Indexed_Models' class from obj_loader

	for (unsigned int i = 0; i < numOf_Vertices; i++) // Loop for every iteration of 'i' in the total number of vertices and increment one
	{
		model.positions.push_back(*Verts[i].Get_Position());  // Push model Vec3 positions on list of vertices
		model.textureCoordinates.push_back(*Verts[i].Get_Texture_Coordinates()); // Push model Vec2 texture coordinates on list of vertices
		model.normals.push_back(*Verts[i].Get_Normal()); // Push model Vec3 normals on list of vertices
	}

	for (unsigned int i = 0; i < numOf_Indices; i++) // Loop for every iteration of 'i' in the total number of indices and increment one
		
		model.indices.push_back(indices[i]); // Push model Vec3 indices on list of vertices

	Initialize_Model(model); // Initialize mesh model
}

// Initialize mesh model from index list of mesh models
void Mesh::Initialize_Model(const Indexed_Models& model)
{
	draw_Count = model.indices.size(); // Model mesh indices size is equal to draw count

	glGenVertexArrays(1, &vertex_ArrayObject); // Store vertex array in vertex_ArrayObject
	glBindVertexArray(vertex_ArrayObject); // Bind vertex_ArrayObject

	glGenBuffers(NUM_OF_BUFFERS, vertex_ArrayBuffers); // Generate buffer from array of buffers

	glBindBuffer(GL_ARRAY_BUFFER, vertex_ArrayBuffers[POSITION_VERTEX_BUFFER]); // Establish position vertex buffer data type and pass it
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW); // Pass the position vertex buffer data to the GPU (buffer data type, buffer data size, data pointer from starting address)
	
	glEnableVertexAttribArray(0); // Enable vertex array attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Vertex array attribute pointer

	glBindBuffer(GL_ARRAY_BUFFER, vertex_ArrayBuffers[TEXTURECOORDINATES_VERT_BUFFER]); // Establish texture coordinates vertex buffer data type and pass it
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.textureCoordinates[0]), &model.textureCoordinates[0], GL_STATIC_DRAW); // Pass the texture coordinates vertex buffer data to the GPU (buffer data type, buffer data size, data pointer from starting address)
	
	glEnableVertexAttribArray(1); // Enable vertex array attributes
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); // Vertex array attribute pointer

	glBindBuffer(GL_ARRAY_BUFFER, vertex_ArrayBuffers[NORMAL_VERT_BUFFER]); // Establish position vertex buffer data type and pass it
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW); // Pass the normals vertex buffer data to the GPU (buffer data type, buffer data size, data pointer from starting address)
	
	glEnableVertexAttribArray(2); // Enable vertex array attributes
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0); // Vertex array attribute pointer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_ArrayBuffers[INDEX_VERT_BUFFER]); // Establish position vertex buffer data type and pass it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW); // Pass the indices vertex buffer data to the GPU (buffer data type, buffer data size, data pointer from starting address)

	glBindVertexArray(0); // Unbind vertex_ArrayObject
}

// Mesh Constructor
Mesh::Mesh()
{
	draw_Count = NULL; // Set draw count to equal NULL
}

// Load mesh model from file name
void Mesh::Load_Model(const std::string& filename)
{
	Indexed_Models model = Object_Model(filename).To_Indexed_Model(); // Mesh obj model data from file source is sent to the indexed model
	Initialize_Model(model); // Initialize the mesh model
	Sphere meshSphere(); // 'meshSphere' function accesses Sphere struct's functionalities
}

// Mesh de-Constructor
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertex_ArrayObject); // Delete arrays
}

// Draw obj model mesh
void Mesh::draw()
{
	glBindVertexArray(vertex_ArrayObject); // Bind vertex array

	glDrawElements(GL_TRIANGLES, draw_Count, GL_UNSIGNED_INT, 0); // Draw mesh elements	

	glBindVertexArray(0); // Unbind vertex array
}

// Update mesh sphere Collider data (position and radius)
void Mesh::Update_SphereCol_Data(glm::vec3 position, float sphereRadius)
{
	meshSphere.Set_Position(position); // Set sphere Collider position
	meshSphere.Set_Sphere_Radius(sphereRadius); // Set sphere Collider radius
}

