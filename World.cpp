#include "World.h"
#include "glm/ext/matrix_transform.inl"
#include "glm/fwd.hpp"
#include <memory>

using std::shared_ptr;

int World::_worldInstances = 0;

shared_ptr<World> World::getWorld() {
	if (worldInstance == nullptr) {
		worldInstance = std::make_shared<World>();
	}

	return worldInstance;
}

void World::translateWorldBy(glm::vec3& newPos) {
	glm::translate(mWorldIdentity, newPos);
}

glm::mat4 World::getWorldMatrix() {
	return mWorldIdentity;
}