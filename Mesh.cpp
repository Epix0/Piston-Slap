#include "Mesh.h"
#include <glad/glad.h>
#include <cstddef>

constexpr GLuint totalVertexAttributes = 3;
constexpr void* ptrZero = 0;

void Mesh::PrepareForGL() {
	// Retaining references to any handle other than the VAO is currently not needed
	GLuint EBO, VBO = 0;
	
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertexData.size(), mVertexData.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mElements.size(), mElements.data(), GL_STATIC_DRAW);

	// Position, Normal, TexCoords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptrZero);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	for (GLuint i = 0; i < totalVertexAttributes; ++i) {
		glEnableVertexAttribArray(i);
	}

	glBindVertexArray(0);
}

void Mesh::Draw() const {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mElements.size()), GL_UNSIGNED_INT, ptrZero);
}
