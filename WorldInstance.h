#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class WorldInstance
{
public:
	WorldInstance() : pos(0), scale(1.0f), orientation(0), modelTransform(1.0f) {};
	~WorldInstance() = default;
	WorldInstance(const WorldInstance&) = delete;

	void scaleBy(const float scalar);
	void scaleBy(const glm::vec3& scalar);
	glm::vec3 getSize() const;
	glm::vec3 getPos() const;
	glm::vec3 getOrientation() const;

private:
	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 orientation;
	glm::mat4 modelTransform; // (world-origin * pos) * (size * orientation)
};