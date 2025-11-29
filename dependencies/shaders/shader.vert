#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aColor;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTexCoords;
out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 modelTransformedFromAPos = model * vec4(aPos, 1.f);
    vFragPos = vec3(modelTransformedFromAPos);
    vTexCoords = aTexCoords;
    vColor = aColor;
    vNormal = mat3(transpose(inverse(model))) * aNormal;

   
    gl_Position = projection * view * modelTransformedFromAPos;
}