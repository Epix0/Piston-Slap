#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include <string>
#include <vector>
#include "assimp/material.h"
#include "glad/glad.h"
#include "glm/fwd.hpp"

using std::string, std::vector;

class Model {
public:
	friend class CustomModelImporter;
	Model(const string& modelName = string("Unnamed_Model"))
		: directory(""), mName(modelName), mWorldPos(0), mWorldOrientation(0), mScale(1.0f), mWorldMatrix(1.0f) {};
	
	// MEMBERS
	
	string directory;
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	
	// FUNCTIONS

	// @shader should be activated/used() prior to this call.
	// This function is only concerned with getting the Model on the screen and translated from World space.
	// View, Projection and other uniforms should be set prior to this call.
	void Draw(ShaderProgram& shader) const;

	void setPos(glm::vec3 newPos);

	// Pushes position and rotation properties to final render matrix
	void updateMatrix();

	// @newOrientation is in Euler angles format in DEGREES
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationDeg(glm::vec3 newOrientation);

	// @newOrientation is in Euler angles format in RADIANS
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationRad(glm::vec3 newOrientation);


	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	std::vector<Mesh> mMeshes;
private:
	// Vector of all the Meshes that make up this model.
	std::string mName;
	glm::vec3 mWorldPos;

	// each component will already be in radians
	glm::vec3 mWorldOrientation;
	glm::vec3 mScale;

	// Written to when updateMatrix() is called
	glm::mat4 mWorldMatrix;

	GLuint TextureFromFile(const char* path, const string& directory, bool gamma = false);
};