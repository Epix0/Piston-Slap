#pragma once
#include <glm/glm.hpp>
#include <string>
// World Object is an instance that has displacement properties from world center and scaling

class WorldObject {
public:
	void setScale(float scalar);

	void setScale(const glm::vec3& scale);

	void setPos(glm::vec3 newPos);

	// @newOrientation is in Euler angles DEGREES
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationDeg(glm::vec3 newOrientation);

	// @newOrientation is in Euler angles RADIANS
	// Ensure to call updateMatrix() to finalize this update
	void setOrientationRad(glm::vec3 newOrientation);

	inline glm::vec3 getPos() const { return mPos; }
	inline glm::vec3 getOrientation() const { return mOrientation; }
	inline glm::vec3 getScale() const { return mScale; }
	glm::mat4 getTransform();

	// Modifies anchored state
	void setAnchoredState(bool state);

	bool getAnchoredState() const;

	inline bool isAnchored() const { return mAnchored; };
	
	void setVelocity(glm::vec3 newVelocity);

	void setMatrix(glm::mat4 matrix);

	glm::vec3 getVelocity() const;
	
	// Accepts a local vector and translates it by this world's transform
	glm::vec3 translateLocalVector(glm::vec3 local);
	// Updates mTransform of model's position, scale, and rotation.
	// Called when mShouldComputeTransform is true during getTransform()
	void computeTransform();
private:

	// Final transform
	glm::mat4 mTransform;

	// XYZ placement relative to world
	glm::vec3 mPos;

	// Euler angles in radians
	glm::vec3 mOrientation;

	// XYZ model scales
	glm::vec3 mScale;

	// whether the object abides by gravity
	bool mAnchored;

	bool mShouldComputeTransform;

	glm::vec3 mVelocity;
protected:
	WorldObject() : mTransform(1.0f), mPos(0), mOrientation(0), mScale(1.0f), mAnchored(false), mShouldComputeTransform(true), mVelocity(0.f) {}
	~WorldObject() = default;
};

