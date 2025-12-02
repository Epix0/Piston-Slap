#pragma once
#include "Model.h"
#include "WorldObject.h"
#include "ShaderProgram.h"
#include <string>
#include <memory>

// An Instance is a world unit that is used in the game loop. An Instance
// will have transformation data and mesh data.

class Instance : public WorldObject {
public:
	Instance(const std::string& baseName = "unnamed_instance", std::weak_ptr<Model> pModel = {}) : cmBaseName(baseName), mpModel(pModel) {};

	inline void draw(ShaderProgram& shader) const {
		if(auto pModel = mpModel.lock()) {
			shader.setMat4("model", getWorldTransform());
			pModel->draw(shader);
		}
	};
private:
	// The reference points to a target element from CustomImporter.mImportedModels
	std::weak_ptr<Model> mpModel;

	// The technical alias of this Instance during registration. This is also the key to use when constructing.
	const std::string cmBaseName;
};