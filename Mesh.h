#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

struct Mesh
{
public:
	Mesh(const std::string& meshName) : name(meshName) {};
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> elementList;
	std::string name;
};