#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;  
in vec2 TexCoords;
in vec3 Color;

uniform int texSlot;
uniform sampler2D texturethang[5];
uniform bool hasTex;

void main() {
    if (hasTex) {
        FragColor = texture(texturethang[texSlot], TexCoords.xy);
    } else {
        FragColor = vec4(Color, 1.);
    }
    
} 

