#include "Model.h"
#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.inl"
#include "World.h"
#include "Mesh.h"

const glm::vec3 Unit_Vector_Pitch = glm::vec3(1.0f, 0, 0);
const glm::vec3 Unit_Vector_Yaw = glm::vec3(0, 1.0f, 0);
const glm::vec3 Unit_Vector_Roll = glm::vec3(0, 0, 1.0f);

void Model::draw(ShaderProgram& shader) const {
	shader.setMat4("model", mWorldTransform);
	for (auto& mesh : mMeshes) {
		mesh.draw(shader);
	}
}

void Model::setPos(glm::vec3 newPos) {
	mWorldPos = newPos;
}

void Model::pushTransformUpdate() {
	glm::mat4 model = World::getWorld()->getWorldMatrix();

	model = glm::translate(model, mWorldPos);
	model = glm::rotate(model, mWorldOrientation.x, Unit_Vector_Pitch);
	model = glm::rotate(model, mWorldOrientation.y, Unit_Vector_Yaw);
	model = glm::rotate(model, mWorldOrientation.z, Unit_Vector_Roll);
	model = glm::scale(model, mScale);

	mWorldTransform = model; // set last to ensure thread safety I reckon
}

void Model::setOrientationDeg(glm::vec3 newOrientation) {
	mWorldOrientation = glm::vec3(glm::radians(newOrientation));
}

void Model::setOrientationRad(glm::vec3 newOrientation) {
	mWorldOrientation = newOrientation;
}