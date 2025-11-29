#include "WorldObject.h"
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.inl"
#include "World.h"

const glm::vec3 Unit_Vector_Pitch = glm::vec3(1.0f, 0, 0);
const glm::vec3 Unit_Vector_Yaw = glm::vec3(0, 1.0f, 0);
const glm::vec3 Unit_Vector_Roll = glm::vec3(0, 0, 1.0f);

void WorldObject::setScale(float scalar) {
	mScale = glm::vec3(scalar);
}

void WorldObject::setScale(const glm::vec3& scale) {
	mScale = scale;
}

void WorldObject::setPos(const glm::vec3& newPos) {
	mWorldPos = newPos;
}

void WorldObject::pushTransformUpdate() {
	glm::mat4 WorldObject = World::getWorld()->getWorldMatrix();

	WorldObject = glm::translate(WorldObject, mWorldPos);
	WorldObject = glm::rotate(WorldObject, mWorldOrientation.x, Unit_Vector_Pitch);
	WorldObject = glm::rotate(WorldObject, mWorldOrientation.y, Unit_Vector_Yaw);
	WorldObject = glm::rotate(WorldObject, mWorldOrientation.z, Unit_Vector_Roll);
	WorldObject = glm::scale(WorldObject, mScale);

	mWorldTransform = WorldObject;
}

void WorldObject::setOrientationDeg(glm::vec3 newOrientation) {
	mWorldOrientation = glm::vec3(glm::radians(newOrientation));
}

void WorldObject::setOrientationRad(glm::vec3 newOrientation) {
	mWorldOrientation = newOrientation;
}