#pragma once
#include <string>
#include "glm/fwd.hpp"

using std::string;

class ShaderProgram
{
    // Key: Any friendly handle to the uniform ID
    // Value: The shader-specific uniform variable
    struct StandardUniforms {
        // Clip Space
        string Projection;
        // Camera/Viewing perspective
        string View;
        // World Space
        string Model;
    };
public:
    static StandardUniforms Uniforms;
    
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    ShaderProgram(const string& vertexPath, const string& fragmentPath);
    // use/activate the shader
    void use() const;
       

    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setIntArray(const std::string& name, int key, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec3) const;
    void setVec3(const std::string& name, const float x, const float y, const float z) const;
    void setVec3Array(const std::string& name, unsigned int count, const glm::vec3& vec3) const;
};

