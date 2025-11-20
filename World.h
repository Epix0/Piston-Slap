#pragma once
#include "glm/glm.hpp"
#include <memory>
#include <iostream>

using std::shared_ptr, std::cout;

// PREPROCCESORS

// [BOOL] Used to track the count of World constructs and warn if more than one is made. If more than one World is used, consider setting to false
#define TRACKING_WORLD_INSTANCES true;

class World {
#ifdef TRACKING_WORLD_INSTANCES
	static int _worldInstances;
#endif
public:
	typedef shared_ptr<World> WorldInstance;

	World() : mWorldIdentity(1.0f) {
		#if TRACKING_WORLD_INSTANCES
			if (++_worldInstances > 1) {
				cout << "[TRACKING_WORLD_INSTANCES]: Heads up! More than one World instance was constructed. getWorld() will NOT return this recent instance.\n";
			}
		#endif
	};

	static shared_ptr<World> getWorld();
	void translateWorldBy(glm::vec3& newPos);

	glm::mat4 getWorldMatrix();
private:
	static std::shared_ptr<World> worldInstance;
	glm::mat4 mWorldIdentity;
};
