#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

/*	This structure is the usable data during GL draw calls.
	Meshes are parts of a Model, which comes from Model.h
	It is essential that every member, other than mName, is valid upon index ->
	-> Their values are set during the scene processing in the custom import class.
	-> So don't go indexing them prior to importing everything, in fact, don't even instantiate them outside of the importer class :)

	I know it's bad practice to not have them as a constructor to ensure this intent, but that's the way I made it
*/
struct Mesh
{
public:
	Mesh() : mName("Model") {};
	Mesh(const std::string& meshName) : mName(meshName) {};

	// note to self if I question why vertex storage type is vec3 instead of float:
	// it's easier to expand from each component when I go to apply it to the GL buffer.
	// TODO: decide how a complete vertex buffer should be sorted so that a member can be made during Mesh import to skip the..
	//			..process of converting Assimp's vec3 type to glm, which later is unpacked to floats

	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec3> mVertices;
	std::vector<GLuint> mElements;
	std::string mName;
};