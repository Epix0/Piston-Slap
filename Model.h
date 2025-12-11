#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include <string>
#include <vector>
#include <iostream>
#include "WorldObject.h"

using std::string, std::vector;

class Model {
	struct AABB {
		glm::vec3 mMin;
		glm::vec3 mMax;

		AABB() = default;
		AABB(const glm::vec3& min, const glm::vec3& max) :
			mMin(min), mMax(max) {}
	};
public:
	friend class CustomModelImporter;
	Model() : mMeshes{}, mBounds{} {};
	Model(const Mesh& mesh) : mMeshes{ mesh }, mBounds{} {};
	
	~Model() = default;

	// @shader should be activated/used() prior to this call.
	// This function is only concerned with getting the Model on the screen and translated from World space.
	// View, Projection and other uniforms should be set prior to this call.
	void draw(ShaderProgram& shader) const;
	
	inline const AABB getBounds() const { return mBounds; };

	inline size_t getMeshCount() const { return mMeshes.size(); };
	
	std::vector<Mesh> mMeshes;
private:
	AABB mBounds;

	void setBoundingBox(const AABB& bounds);

	// Model's Meshes


};