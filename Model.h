#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

using std::string, std::vector;

class Model {
public:
	friend class CustomModelImporter;
	Model(const string& modelName = string("Unnamed_Model")) : mName(modelName), mWorldPos(0), mWorldOrientation(0), mScale(1.0f), mWorldMatrix(0) {};

	// @shader should be activated/used() prior to this call.
	// This function is only concerned with getting the Model on the screen and translated from World space.
	// View, Projection and other uniforms should be set prior to this call.
	void Draw(const ShaderProgram& shader) const;

	void setPos(glm::vec3 newPos);
	glm::vec3 getPos() const;
	
	void setOrientation();
private:
	// Vector of all the Meshes that make up this model.
	std::string mName;
	std::vector<Mesh> mMeshes;
	glm::vec3 mWorldPos;
	glm::vec3 mWorldOrientation;
	size_t mScale;

	// Written to when either Pos or Orientation is changed
	glm::mat4 mWorldMatrix;
};