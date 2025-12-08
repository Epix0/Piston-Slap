#pragma once
#include <vector>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include "Texture.h"
#include <memory>
#include <vector>
#include <assimp/vector3.h>

/* Mesh contains the vertex data which makes up its shape and texture placement (texcoords).
* The current data uses triangulated primitives for its position.
* Uses: Texture
*/

class Mesh {
public:
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Color;
		Vertex() : Position(0), Normal(0), TexCoords(0), Color(0) {};
		Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& texcoords, const glm::vec3& col) :
			Position(pos), Normal(norm), TexCoords(texcoords), Color(col)
		{}
	};

	struct AABB {
		glm::vec3 mMin;
		glm::vec3 mMax;

		AABB() : mMin{}, mMax{} {};
		AABB(const aiVector3D& min, const aiVector3D& max) :
			mMin(min.x, min.y, min.z), mMax(max.x, max.y, max.z) {};
		AABB(const glm::vec3& min, const glm::vec3& max) : 
			mMin(min), mMax(max) {
		};
	};
	Mesh() : VAO(0), VBO(0), EBO(0), mVertices{}, mElements{}, mTexture{}, mBounds{} {};

	// prepForOpenGL() will call itself
	Mesh(const std::vector<float>& vertices3f) : VAO(0), VBO(0), EBO(0), mVertices{}, mElements{}, mTexture{}, mBounds{} {
		prepareForGL();
	};

	// Mesh manages ownership of custom Texture. prepForOpenGL() will call itself
	Mesh(const std::vector<float>& vertices3f, std::shared_ptr<Texture>&& pTexture) {
		prepareForGL();
	};;

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
	GLuint VAO, VBO, EBO;

	AABB mBounds;
private:
};