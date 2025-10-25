#pragma once
#include <string>

class PhysicsObject
{
public:
	virtual void setPos() = 0;
	virtual void* getPos() = 0;

	~PhysicsObject() = delete;
};