#include "WorldInstance.h"

void WorldInstance::scaleBy(const float scalar)
{
	this->modelTransform = glm::scale(this->modelTransform, glm::vec3(scalar));
}

void WorldInstance::scaleBy(const glm::vec3& scalar)
{
	this->modelTransform = glm::scale(this->modelTransform, scalar);
}

glm::vec3 WorldInstance::getSize() const
{
	return this->scale;
}

glm::vec3 WorldInstance::getPos() const
{
	return this->pos;
}

glm::vec3 WorldInstance::getOrientation() const
{
	return this->orientation;
}