#pragma once
#include "Model.h"
#include "WorldObject.h"
#include "ShaderProgram.h"
#include <string>
#include <memory>
#include <utility>
#include "CustomIModelImporter.h"

// An Instance is a world unit that is used in the game loop. An Instance
// will have transformation data and mesh data.

class Instance : public WorldObject {
protected:
	Instance() : mFriendlyName("Unnamed_Instance"), mpModel{} {};
	Instance(std::weak_ptr<Model> pModel) : mFriendlyName("Unnamed_Instance"), mpModel(pModel) {};
public:
	virtual ~Instance() = default;

	virtual inline void draw(ShaderProgram& shader) {
		if(auto pModel = mpModel.lock()) {
			shader.setMat4("model", getTransform());
			pModel->draw(shader);
		}
	};

	virtual std::unique_ptr<Instance> clone() const = 0;

	virtual inline void setFriendlyName(std::string newName) {
		mFriendlyName = std::move(newName);
	}

	virtual inline std::string getFriendlyName() const {
		return mFriendlyName;
	}

	inline std::shared_ptr<Model> getModel() const {
		return mpModel.lock();
	}

	inline void setModel(std::weak_ptr<Model> pModel) {
		mpModel = std::move(pModel);
	}
private:
	// The reference points to a target element from CustomImporter.mImportedModels
	std::weak_ptr<Model> mpModel;
	std::string mFriendlyName;
};