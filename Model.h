#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include <string>
#include <vector>
#include "glad/glad.h"
#include "glm/fwd.hpp"

using std::string, std::vector;

class Model {
public:
	friend class CustomModelImporter;
	Model(const string& modelName = string("Unnamed_Model"))
		: mName(modelName), mWorldTransform(1.0f), mWorldPos(0), mWorldOrientation(0), mScale(1.0f) {};
	
	~Model() = default;

	// @shader should be activated/used() prior to this call.
	// This function is only concerned with getting the Model on the screen and translated from World space.
	// View, Projection and other uniforms should be set prior to this call.
	void draw(ShaderProgram& shader) const;

	void setPos(glm::vec3 newPos);

	// Updates mWorldTransform of model's position, scale, and rotation.
	// Intended to be called at the end of all transform-related updates.
	void pushTransformUpdate();

	// @newOrientation is in Euler angles DEGREES
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationDeg(glm::vec3 newOrientation);

	// @newOrientation is in Euler angles RADIANS
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationRad(glm::vec3 newOrientation);

	inline glm::vec3 getPos() const { return mWorldPos; }
	inline glm::vec3 getOrientation() const { return mWorldOrientation; }
	inline glm::vec3 getScale() const { return mScale; }
	inline size_t getMeshCount() const { return mMeshes.size(); }

	// Technical alias, though not a friendly identifier at times
	std::string mName;
private:
	// A matrix with the World identity comprised of Model's scale, position, and rotation.
	// Written to when updateMatrix() is called
	glm::mat4 mWorldTransform;

	// XYZ placement relative to world
	glm::vec3 mWorldPos;

	// Euler angles in radians
	glm::vec3 mWorldOrientation;
	
	// XYZ model scale
	glm::vec3 mScale;

	// Model's Meshes
	std::vector<Mesh> mMeshes;
};