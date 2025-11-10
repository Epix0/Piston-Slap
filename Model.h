#pragma once
#include "Mesh.h"
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include <glad/glad.h>

class Model {
public:
	friend class CustomModelImporter;
	Model(const std::string& modelName = std::string("Unnamed_Model")) : mNumOfMeshes(0), mName(modelName) {}
	Model(const Model&) = delete;

	void Draw(ShaderProgram& shaderProgam);
private:
	enum BufferId {
		VAO,
		VBO,
		EBO,
		_total_ids
	};

	static GLuint mBufferIdAssignment;

	// This will be the size of mMeshes during iteration
	unsigned int mNumOfMeshes;
	// Vector of all the Meshes that make up this model.
	std::string mName;
	std::vector<Mesh> mMeshes;
	// Rendering stuffs
	std::vector<GLuint> mBuffers;

	// Returns and increments mBufferIdAssignment
	GLuint GetBufferId();
};