#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class CustomModelImporter {
public:
	CustomModelImporter(): vertices{ 0 }, numOfVertices(0), numOfFaces(0) {};
	float vertices[6000];

	bool ImportModelFile(const std::string& pFile);
	unsigned int numOfVertices;
	unsigned int numOfFaces;
	std::vector<unsigned int> elementList;
private:
	aiMesh* importedMeshData;
	void ProcessScene(const aiScene* scene);
};