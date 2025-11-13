#include "Model.h"
#include <iostream>

GLuint Model::mBufferIdAssignment = 1;
using std::cout;

void Model::Draw(){
	for (size_t i = 0; i < mNumOfMeshes; ++i) {
		//shaderProgam.use();
		glBindVertexArray(mVAOs[i]);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mMeshes[i].mElements.size()), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, mMeshes[i].mVertices.size());
	}
}

//std::cout << "Fuckin random check: " << glGetError() << "\n";
void Model::PrepGLBuffers(){
	if (mNumOfMeshes <= 0) {
		std::cout << "PrepGLBuffers() was called without valid Meshes loaded\n";
		return;
	}
	mVAOs.reserve(mNumOfMeshes);
	mVBOs.reserve(mNumOfMeshes);
	mEBOs.reserve(mNumOfMeshes);

	for (GLuint i = 0; i < mNumOfMeshes; ++i) {
		GLuint vao = 0;
		GLuint vbo = 0;
		GLuint ebo = 0;

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		mVAOs[i] = vao;
		mVBOs[i] = vbo;
		mEBOs[i] = ebo;

		glBindVertexArray(vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		Mesh& mesh = mMeshes[i];
		std::vector<float> joinedVertexData;
		joinedVertexData.reserve((mesh.mVertices.size() * 3) * 2); // 2 to account for mVertices and mNormals; 3 to account for each component

		for (size_t i = 0; i < mesh.mVertices.size(); ++i) {
			glm::vec3& vertex = mesh.mVertices[i];
			glm::vec3& normal = mesh.mNormals[i];
			joinedVertexData.insert(joinedVertexData.end(), { vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z });
		}

		// vertex geometry, vertex normals
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.mElements.size(), mesh.mElements.data(), GL_STATIC_DRAW);
		std::cout << "Element buffering: " << glGetError() << "\n";

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * joinedVertexData.size(), joinedVertexData.data(), GL_DYNAMIC_DRAW);
		std::cout << "VBO buffering: " << glGetError() << "\n";
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0); // Geometry
		std::cout << "VBO attributes: " << glGetError() << "\n";
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*) (sizeof(float) * 3)); // Normals
		std::cout << "Element attributes: " << glGetError() << "\n";
		glEnableVertexAttribArray(1);
	}


	mAreBuffersLoaded = true;
}

GLuint Model::GetABufferId()
{
	return mBufferIdAssignment++;
}
