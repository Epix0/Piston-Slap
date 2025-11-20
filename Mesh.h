#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

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
};

class Mesh {
public:
	Mesh(): VAO(0) {
	}

	void PrepareForGL();

	// Utilizes the active shader. *All* uniforms should be set prior to this call
	void Draw() const;
	// Comprised of all the data used in the VBO
	vector<Vertex> mVertexData;
	vector<GLuint> mElements;
	
private:
	GLuint VAO;
};