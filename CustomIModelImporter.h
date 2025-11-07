#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "Mesh.h"

class CustomModelImporter {
public:
	CustomModelImporter(): numOfVertices(0), sizeSortedBuffer(0) {};
	unsigned int numOfVertices;
	size_t sizeSortedBuffer;

	bool ImportModelFile(const std::string& pFile);
	std::unique_ptr<Mesh>&& getDecodedMesh();

	//std::vector<unsigned int> elementList;
private:
	void ProcessScene(const aiScene* scene);
	std::unique_ptr<Mesh> decodedMesh;
};