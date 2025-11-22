#include "Mesh.h"
#include "ShaderProgram.h"
#include <glad/glad.h>
#include <cstddef>

#include <iostream>

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertexData.size(), mVertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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

void Mesh::Draw(ShaderProgram& shader) const {
	GLuint diffuseNr = 1;
	GLuint heightNr = 1;
	GLuint normalNr = 1;
	GLuint specularNr = 1;

	// tex proc
    for (GLuint i = 0; i < mTextureData.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = mTextureData[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, mTextureData[i].id);
    }

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mElements.size()), GL_UNSIGNED_INT, ptrZero);
}
