#pragma once
#include "Model.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include <string>
#include <vector>

class CustomModelImporter {
public:
	CustomModelImporter(): numOfVertices(0), sizeSortedBuffer(0) {};
	unsigned int numOfVertices;
	size_t sizeSortedBuffer;

	bool ImportModelFile(const std::string& pFile);
	std::vector<Model> mImportedModels;
private:
	// Intended to be called during mai nmesh processing and before GL prep calls
	void processVertices(const aiMesh& sceneMesh, Mesh& meshOfModel) const;

	// Intended to be called during main Mesh processing and before GL prep calls
	void proccessElements(const aiMesh& sceneMesh, Mesh& meshOfModel) const;

	// Intended to be called during Node processing for the Mesh phase
	void processAIMesh(const aiMesh& sceneMesh, Model& parentModel) const;

	// Intended to be called after entering Scene processing
	void processNodeRecursively(const aiNode* node, Model& model, const aiScene* scene) const;

	// Main entry point for multi-phased processesing
	void processScene(const aiScene* scene, std::string&& modelName);
};