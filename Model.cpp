#include "Model.h"
#include "ShaderProgram.h"
#include "glm/ext/matrix_transform.inl"
#include "World.h"
#include "stb/stb_image.h"
#include "assimp/material.h"
#include "assimp/types.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Mesh.h"
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

const glm::vec3 Unit_Vector_Pitch = glm::vec3(1.0f, 0, 0);
const glm::vec3 Unit_Vector_Yaw = glm::vec3(0, 1.0f, 0);
const glm::vec3 Unit_Vector_Roll = glm::vec3(0, 0, 1.0f);

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
   
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

GLuint Model::TextureFromFile(const char* path, const string& directory, bool gamma) {
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model::Draw(ShaderProgram& shader) const {
	shader.setMat4(ShaderProgram::Uniforms.Model, mWorldMatrix);
	for (auto& mesh : mMeshes) {
		mesh.Draw(shader);
	}
}

void Model::setPos(glm::vec3 newPos) {
	mWorldPos = newPos;
}

void Model::updateMatrix() {
	glm::mat4 model = World::getWorld()->getWorldMatrix();

	model = glm::translate(model, mWorldPos);
	model = glm::rotate(model, mWorldOrientation.x, Unit_Vector_Pitch);
	model = glm::rotate(model, mWorldOrientation.y, Unit_Vector_Yaw);
	model = glm::rotate(model, mWorldOrientation.z, Unit_Vector_Roll);
	model = glm::scale(model, mScale);

	mWorldMatrix = model; // set last to ensure thread safety I reckon
}

void Model::setOrientationDeg(glm::vec3 newOrientation) {
	mWorldOrientation = glm::vec3(glm::radians(newOrientation));
}

void Model::setOrientationRad(glm::vec3 newOrientation) {
	mWorldOrientation = newOrientation;
}

glm::vec3 Model::getPos() const {
    return mWorldPos;
}
