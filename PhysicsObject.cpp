#include "PhysicsObject.h"
#include <initializer_list>

void PhysicsObject::setPos(float x, float y, float z)
{
	this->currentPosition = std::make_shared<std::vector<float>>(std::initializer_list<float>{x, y, z});
}

std::shared_ptr<std::vector<float>> PhysicsObject::getPos()
{
	if (this->currentPosition == nullptr) {
		std::lock_guard<std::mutex> localLock(this->mutex_currentPosition);
		if (this->currentPosition == nullptr) {
			this->setPos(0, 0, 0);
		}
	}

	return this->currentPosition;
}

void PhysicsObject::setGravityState(bool newState)
{
}
