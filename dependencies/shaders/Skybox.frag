#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube texturethang;

void main()
{    
    FragColor = texture(texturethang, TexCoords);
}