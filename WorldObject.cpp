#include "WorldObject.h"
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.inl"
#include "World.h"

const glm::vec3 Unit_Vector_Pitch = glm::vec3(1.0f, 0, 0);
const glm::vec3 Unit_Vector_Yaw = glm::vec3(0, 1.0f, 0);
const glm::vec3 Unit_Vector_Roll = glm::vec3(0, 0, 1.0f);

void WorldObject::setScale(float scalar) {
	mShouldComputeTransform = true;
	mScale = glm::vec3(scalar);
}

void WorldObject::setScale(const glm::vec3& scale) {
	mShouldComputeTransform = true;
	mScale = scale;
}

void WorldObject::setPos(glm::vec3 newPos) {
	mShouldComputeTransform = true;
	mPos = std::move(newPos);
}

void WorldObject::computeTransform() {
	glm::mat4 worldInstance = World::getWorld()->getWorldMatrix();

	worldInstance = glm::translate(worldInstance, mPos);
	worldInstance = glm::rotate(worldInstance, mOrientation.x, Unit_Vector_Pitch);
	worldInstance = glm::rotate(worldInstance, mOrientation.y, Unit_Vector_Yaw);
	worldInstance = glm::rotate(worldInstance, mOrientation.z, Unit_Vector_Roll);
	worldInstance = glm::scale(worldInstance, mScale);

	mTransform = worldInstance;
}

void WorldObject::setVelocity(glm::vec3 newVelocity) {
	mVelocity = std::move(newVelocity);
}

glm::vec3 WorldObject::getVelocity() const {
	return mVelocity;
}

void WorldObject::setOrientationDeg(glm::vec3 newOrientation) {
	mShouldComputeTransform = true;
	mOrientation = glm::vec3(glm::radians(newOrientation));
}

void WorldObject::setOrientationRad(glm::vec3 newOrientation) {
	mShouldComputeTransform = true;
	mOrientation = newOrientation;
}

glm::mat4 WorldObject::getTransform() {
	if(mShouldComputeTransform) {
		mShouldComputeTransform = false;
		computeTransform();
	}

	return mTransform;
}

// Responsible for adding and removing objects from gravity stepping
void WorldObject::setAnchoredState(bool state) {
	mAnchored = state;
}

bool WorldObject::getAnchoredState() const {
	return mAnchored;
}