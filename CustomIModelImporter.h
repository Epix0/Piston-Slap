#pragma once
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include "Model.h"

class CustomModelImporter {
public:
	CustomModelImporter(): numOfVertices(0), sizeSortedBuffer(0) {};
	unsigned int numOfVertices;
	size_t sizeSortedBuffer;

	bool ImportModelFile(const std::string& pFile);
	std::vector<Model> mImportedModels;
private:

	// Intended to be used recursively.
	// @param parentModel ref is where the node will shove its data
	void ProcessNode(const aiNode* node, Model& model, const aiScene* scene) const;

	void ProcessScene(const aiScene* scene, std::string&& modelName);
};