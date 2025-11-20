#include "Model.h"
#include "ShaderProgram.h"
#include "glm/ext/matrix_transform.inl"
#include "World.h"
#include "glm/fwd.hpp"

void Model::Draw(const ShaderProgram& shader) const {
	shader.setMat4(ShaderProgram::Uniforms.Model, mWorldMatrix);
	
	for (auto& mesh : mMeshes) {
		mesh.Draw();
	}
}

void Model::setPos(glm::vec3 newPos) {
	mWorldPos = newPos;
	mWorldMatrix = glm::translate(World::getWorld()->getWorldMatrix(), mWorldPos);
}
