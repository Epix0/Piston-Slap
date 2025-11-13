#pragma once
#include "Mesh.h"
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include <glad/glad.h>

class Model {
public:
	friend class CustomModelImporter;
	Model(const std::string& modelName = std::string("Unnamed_Model")) : mAreBuffersLoaded(false), mNumOfMeshes(0), mName(modelName) {};

	/*
		@brief Runs every dra
	*/
	void Draw();
	void PrepGLBuffers();
private:
	enum BufferId {
		VAO,
		VBO,
		EBO,
		_total_ids
	};

	static GLuint mBufferIdAssignment;
	// Default: false. Becomes true after calling PrepGLBuffers()
	bool mAreBuffersLoaded;
	// This will be the size of mMeshes during iteration
	size_t mNumOfMeshes;
	// Vector of all the Meshes that make up this model.
	std::string mName;
	std::vector<Mesh> mMeshes;
	
	//**
	// These members are meant to store Mesh-specific GL buffers.
	// These members are in order, where retrieving the buffers for a 0-index Mesh from mMesh..
	// is as simple as using that index in each of these vectors (mVAOs[0] for Mesh #1)
	
	std::vector<GLuint> mVAOs;
	std::vector<GLuint> mVBOs;
	std::vector<GLuint> mEBOs;

	// **

	// Returns and increments mBufferIdAssignment
	GLuint GetABufferId();
};