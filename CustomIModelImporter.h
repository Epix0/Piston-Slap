#pragma once
#include "Model.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include <string>
#include "Mesh.h"
#include <map>
#include <filesystem>
#include <memory>

class CustomModelImporter {
public:
	enum class NativeModelNames : char {
		Cube,
		AlphaCharacter,
	};

	static CustomModelImporter& get() {
		static CustomModelImporter v;
		return v;
	}

	bool ImportModelFile(const std::filesystem::path& fileSysPath);

	std::weak_ptr<Model> getModel(const std::string& modelName);

	std::weak_ptr<Model> getModel(CustomModelImporter::NativeModelNames eName);

	std::map<std::string, std::shared_ptr<Model>> mImportedModels;

	CustomModelImporter(const CustomModelImporter&) = delete;
	CustomModelImporter& operator=(const CustomModelImporter&) = delete;
private:
	CustomModelImporter() : mImportedModels{} {};

	static constexpr std::string modelEnumToString(CustomModelImporter::NativeModelNames eName) {
		switch(eName) {
		case CustomModelImporter::NativeModelNames::Cube:
			return "cube";
		case CustomModelImporter::NativeModelNames::AlphaCharacter:
			return "character";
		default:
			return "unnamed";
		}
	};

	// The arrangement of process*() args should be as follows: primary scene object; Mesh OR Model; optionally, the scene ptr itself

	// The beginning of texture handling...
	void processTextures(const aiMesh& sceneMesh, Mesh& meshOfModel, const aiScene* scene, const std::string& modelName) const;

	// Intended to be called during mai nmesh processing and before GL prep calls
	// Anything that will be retreived by an attribute should be here
	void processVertices(const aiMesh& sceneMesh, Mesh& meshOfModel, const aiScene* scene) const;

	// Intended to be called during main Mesh processing and before GL prep calls
	void proccessElements(const aiMesh& sceneMesh, Mesh& meshOfModel) const;

	// Intended to be called during Node processing for the Mesh phase
	void processAIMesh(const aiMesh& sceneMesh, Model& parentModel, const aiScene* scene, const std::string& modelName) const;

	// Intended to be called after entering Scene processing
	void processNodeRecursively(const aiNode* node, Model& model, const aiScene* scene, const std::string& modelName) const;

	// Main entry point for multi-phased processesing
	void processScene(const aiScene* scene, const std::string& modelName);
};