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
	virtual ~Instance() = default;

	virtual inline void draw(ShaderProgram& shader) {
		if(auto pModel = mpModel.lock()) {
			shader.setMat4("model", getTransform());
			pModel->draw(shader);
		}
	};

	virtual std::unique_ptr<Instance> clone() const = 0;

	inline std::shared_ptr<Model> getModel() const {
		return mpModel.lock();
	}
private:
	// The reference points to a target element from CustomImporter.mImportedModels
	std::weak_ptr<Model> mpModel;

	// The technical alias of this Instance during registration. This is also the key to use when constructing.
	const std::string cmBaseName;
};