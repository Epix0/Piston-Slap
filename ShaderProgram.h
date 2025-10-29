#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class ShaderProgram
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use() const;
    // utility uniform functions

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3Array(const std::string& name, unsigned int count, const glm::vec3& vec) const;
};

