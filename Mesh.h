#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"

using std::string, std::vector;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	Vertex() = default;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};


class Mesh {
public:
	Mesh(): VAO(0) {
	}

	void PrepareForGL();

	// Shader should be active already.
	// @shader is passed so that mesh-specific textures are set to their uniforms accordingly
	void Draw(ShaderProgram& shader) const;
	// Comprised of all the data used in the VBO
	vector<Vertex> mVertexData;
	vector<GLuint> mElements;
	vector<Texture> mTextureData;
	
private:
	GLuint VAO;
};