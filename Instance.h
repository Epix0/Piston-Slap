#pragma once
#include "Model.h"
#include "WorldObject.h"
#include <string>

// An Instance is a world unit that is used in the game loop. An Instance
// will have transformation data and mesh data.

class Instance {
public:
	Instance(const Model& model);
private:
	// The reference points to a target element from CustomImporter.mImportedModels
	const Model& cmModelRef;

	// The technical alias of this Instance during registration. This is also the key to use when constructing.
	const std::string cmBaseName;
};