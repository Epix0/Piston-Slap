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
	Model(const string& modelName = string("Unnamed_Model"))
		: mName(modelName), mWorldPos(0), mWorldOrientation(0), mScale(1.0f), mWorldMatrix(1.0f){};

	// @shader should be activated/used() prior to this call.
	// This function is only concerned with getting the Model on the screen and translated from World space.
	// View, Projection and other uniforms should be set prior to this call.
	void Draw(const ShaderProgram& shader) const;

	void setPos(glm::vec3 newPos);
	
	// Pushes position and rotation properties to final render matrix
	void updateMatrix();
	glm::vec3 getPos() const;
	
	// @newOrientation is in Euler angles format in DEGREES
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationDeg(glm::vec3 newOrientation);

	// @newOrientation is in Euler angles format in RADIANS
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationRad(glm::vec3 newOrientation);
private:
	// Vector of all the Meshes that make up this model.
	std::string mName;
	std::vector<Mesh> mMeshes;
	glm::vec3 mWorldPos;

	// each component will already be in radians
	glm::vec3 mWorldOrientation;
	glm::vec3 mScale;

	// Written to when either Pos or Orientation is changed
	glm::mat4 mWorldMatrix;
};