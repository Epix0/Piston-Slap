#include "Mesh.h"
#include "ShaderProgram.h"
#include <glad/glad.h>
#include <cstddef>

#include <iostream>

constexpr GLuint totalVertexAttributes = 4;
constexpr void* ptrZero = 0;

Mesh::~Mesh() {
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::prepareForGL() {
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

	if(mElements.size() > 0) {
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mElements.size(), mElements.data(), GL_STATIC_DRAW);
	}

	// Position, Normal, TexCoords, Color
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptrZero);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

	for (GLuint i = 0; i < totalVertexAttributes; ++i) {
		glEnableVertexAttribArray(i);
	}

	glBindVertexArray(0);
}

void Mesh::draw(ShaderProgram& shader) const {
	if (mTexture) {
		shader.setBool("hasTex", true);
		shader.setInt("texSlot", mTexture->getAssignedTextureSlot());
		shader.setIntArray("texturethang", mTexture->getAssignedTextureSlot(), mTexture->getAssignedTextureSlot());
		mTexture->bind();
	} else
		shader.setBool("hasTex", false);

	glBindVertexArray(VAO);
	if(mElements.size() > 0)
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mElements.size()), GL_UNSIGNED_INT, ptrZero);
	else
		glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

	//glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);
}
