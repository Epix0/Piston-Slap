#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

/*	This structure is the usable data during GL draw calls.
	Meshes are parts of a Model, which comes from Model.h
*/
struct Mesh
{
public:
	Mesh(const std::string& meshName) : mMame(meshName) {};
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec3> mVertices;
	std::vector<unsigned int> mElements;
	std::string mMame;
};