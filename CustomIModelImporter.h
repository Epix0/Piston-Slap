#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class CustomModelImporter {
public:
	CustomModelImporter() = default;
	float vertices[3000];

	bool ImportModelFile(const std::string& pFile);
	unsigned int numOfVertices;
private:
	aiMesh* importedMeshData;
	void ProcessScene(const aiScene* scene);
};