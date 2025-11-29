#pragma once
#include <glm/glm.hpp>
#include <string>
// World Object is an instance that has displacement properties from world center and scaling

class WorldObject {
public:
	WorldObject(const std::string& objectName="Unnamed_WorldObject") : mName(objectName), mWorldTransform(1.0f), mWorldPos(0), mWorldOrientation(0), mScale(1.0f) {}

	void setPos(glm::vec3 newPos);

	// Updates mWorldTransform of model's position, scale, and rotation.
	// Intended to be called at the end of all transform-related updates.
	void pushTransformUpdate();

	// @newOrientation is in Euler angles DEGREES
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationDeg(glm::vec3 newOrientation);

	// @newOrientation is in Euler angles RADIANS
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationRad(glm::vec3 newOrientation);

	inline glm::vec3 getPos() const { return mWorldPos; }
	inline glm::vec3 getOrientation() const { return mWorldOrientation; }
	inline glm::vec3 getScale() const { return mScale; }
	inline glm::mat4 getWorldTransform() const { return mWorldTransform; };

	// Technical alias, though not a friendly identifier at times
	std::string mName;
private:
	// A matrix with the World identity comprised of Model's scale, position, and rotation.
// Written to when updateMatrix() is called
	glm::mat4 mWorldTransform;

	// XYZ placement relative to world
	glm::vec3 mWorldPos;

	// Euler angles in radians
	glm::vec3 mWorldOrientation;

	// XYZ model scale
	glm::vec3 mScale;
};

