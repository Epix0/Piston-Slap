#include "Model.h"
#include "ShaderProgram.h"
#include "Mesh.h"

void Model::draw(ShaderProgram& shader) const {
	for(auto& mesh : mMeshes) {
		mesh.draw(shader);
	}
}