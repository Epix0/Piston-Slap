#include "Model.h"
#include "ShaderProgram.h"
#include "Mesh.h"

void Model::draw(ShaderProgram& shader) const {
	shader.setMat4("model", getWorldTransform());
	for(auto& mesh : mMeshes) {
		mesh.draw(shader);
	}
}