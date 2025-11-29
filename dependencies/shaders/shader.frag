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
        vec4 texColor = texture(texturethang[texSlot], TexCoords.xy);
        if (texColor.a < 0.1)
            discard;
        FragColor = texColor;
    } else {
        FragColor = vec4(Color, 1.);
    }
    
} 

