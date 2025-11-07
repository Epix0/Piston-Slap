#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

struct Mesh
{
public:
	Mesh(const std::string& meshName) : normals{ 0 }, vertices{ 0 }, elementList{ 0 }, name(meshName) {};
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> elementList;
	std::string name;
};