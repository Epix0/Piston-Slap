#pragma once
#include "Model.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include <string>
#include <vector>
#include "Mesh.h"

class CustomModelImporter {
public:
	CustomModelImporter(): numOfVertices(0), sizeSortedBuffer(0) {};
	unsigned int numOfVertices;
	size_t sizeSortedBuffer;

	bool ImportModelFile(const std::string& pFile);
	std::vector<Model> mImportedModels;
private:
	// The arrangement of process*() args should be as follows: primary scene object; Mesh OR Model; optionally, the scene ptr itself

	// The beginning of material handling...
	void processMaterials(const aiMesh* sceneMesh, Mesh* meshOfModel, const aiScene* scene) const;

	// The beginning of texture handling...
	void processTextures(const aiMesh& sceneMesh, Mesh& meshOfModel, const aiScene* scene) const;

	// Intended to be called during mai nmesh processing and before GL prep calls
	// Anything that will be retreived by an attribute should be here
	void processVertices(const aiMesh& sceneMesh, Mesh& meshOfModel, const aiScene* scene) const;

	// Intended to be called during main Mesh processing and before GL prep calls
	void proccessElements(const aiMesh& sceneMesh, Mesh& meshOfModel) const;

	// Intended to be called during Node processing for the Mesh phase
	void processAIMesh(const aiMesh& sceneMesh, Model& parentModel, const aiScene* scene) const;

	// Intended to be called after entering Scene processing
	void processNodeRecursively(const aiNode* node, Model& model, const aiScene* scene) const;

	// Main entry point for multi-phased processesing
	void processScene(const aiScene* scene, std::string&& modelName);
};