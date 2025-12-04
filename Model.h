#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include <string>
#include <vector>
#include <iostream>
#include "WorldObject.h"

using std::string, std::vector;

class Model {
public:
	friend class CustomModelImporter;
	Model() : mMeshes{} {};
	Model(const Mesh& mesh) : mMeshes{mesh} {};
	
	Model(const Model&) {
		std::cout << "Model copied\n";
	}

	~Model() = default;

	// @shader should be activated/used() prior to this call.
	// This function is only concerned with getting the Model on the screen and translated from World space.
	// View, Projection and other uniforms should be set prior to this call.
	void draw(ShaderProgram& shader) const;
		
	inline size_t getMeshCount() const { return mMeshes.size(); };
private:
	// Model's Meshes
	std::vector<Mesh> mMeshes;
};