#pragma once
#include <iostream>
class PhysicsObject
{
public:
	PhysicsObject(const PhysicsObject&) = delete;

	~PhysicsObject();
};