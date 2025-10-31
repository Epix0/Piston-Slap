#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class CustomModelImporter {
public:
	CustomModelImporter(): numOfVertices(0), numOfFaces(0) {};
	unsigned int numOfVertices;
	unsigned int numOfFaces;

	bool ImportModelFile(const std::string& pFile);
	std::vector<float> normals;
	std::vector<float> vertices;
	std::vector<unsigned int> elementList;
private:
	void ProcessScene(const aiScene* scene);
};