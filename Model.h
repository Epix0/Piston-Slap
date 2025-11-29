#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include <string>
#include <vector>
#include "WorldObject.h"
#include <iostream>

using std::string, std::vector;

class Model : public WorldObject {
public:
	friend class CustomModelImporter;
	Model(const string& modelName = string("Unnamed_Model")) {
		mName = modelName;
	};
	
	Model(const Model&) {
		std::cout << "Model copied\n";
	}

	~Model() = default;

	// @shader should be activated/used() prior to this call.
	// This function is only concerned with getting the Model on the screen and translated from World space.
	// View, Projection and other uniforms should be set prior to this call.
	void draw(ShaderProgram& shader) const;
		
	inline size_t getMeshCount() const { return mMeshes.size(); }


private:
	// Model's Meshes
	std::vector<Mesh> mMeshes;
};