#pragma once
#include <string>
#include <memory>
#include <vector>
#include <mutex>

class PhysicsObject
{
public:
	virtual void setPos(float x, float y, float z);
	virtual std::shared_ptr<std::vector<float>> getPos();
	virtual void setGravityState(bool newState);

	~PhysicsObject() = delete;
private:
	std::mutex mutex_currentPosition;
	std::shared_ptr<std::vector<float>> currentPosition;
	bool gravityState;
};