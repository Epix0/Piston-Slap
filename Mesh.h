#pragma once
#include <vector>
#include <memory>
#include <string>

struct Mesh
{
public:
	Mesh(const std::string& meshName) : normals{ 0 }, vertices{ 0 }, elementList{ 0 }, name(meshName) {};
	std::vector<float> normals;
	std::vector<float> vertices;
	std::vector<unsigned int> elementList;
	std::string name;
};