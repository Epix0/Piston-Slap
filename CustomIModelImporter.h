#pragma once
#include "Model.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include <string>
#include "Mesh.h"
#include <map>
#include <filesystem>

class CustomModelImporter {
public:
	CustomModelImporter(): numOfVertices(0) {};
	unsigned int numOfVertices;

	bool ImportModelFile(const std::filesystem::path& fileSysPath);

	Model& getModel(const std::string& modelName);

	std::map<std::string, Model> mImportedModels;
private:

	// The arrangement of process*() args should be as follows: primary scene object; Mesh OR Model; optionally, the scene ptr itself

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
	void processScene(const aiScene* scene, const std::string& modelName);
};