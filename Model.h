#pragma once
#include "Mesh.h"
#include "vector"
#include "string"

struct Model {
public:
	Model() : mNumOfMeshes(0) {}

	// This will be the size of mMeshes during iteration
	unsigned int mNumOfMeshes;
	// Vector of all the Meshes that make up this model.
	std::vector<Mesh> mMeshes;
	std::string mName;
};