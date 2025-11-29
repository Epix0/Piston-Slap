#pragma once
#include <vector>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include "Texture.h"
#include <memory>

/* Mesh contains the vertex data which makes up its shape and texture placement (texcoords).
* The current data uses triangulated primitives for its position.
* Uses: Texture
*/

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Color;
};

class Mesh {
public:
	Mesh() : VAO(0), VBO(0), EBO(0) {};

	~Mesh();

	void prepareForGL();

	// Shader should be active already.
	// @shader is passed so that mesh-specific textures are set to their uniforms accordingly
	void draw(ShaderProgram& shader) const;
	// Comprised of all the data used in the VBO. See struct Vertex
	std::vector<Vertex> mVertices;
	// An array of indices used to index mVertices
	std::vector<GLuint> mElements;
	// Only loading diffuse maps sooo one member lol
	std::shared_ptr<Texture> mTexture;
private:
	GLuint VAO, VBO, EBO;
};