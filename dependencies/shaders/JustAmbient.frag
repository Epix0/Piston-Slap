#version 330 core
out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;  
in vec2 vTexCoords;
in vec3 vColor;

uniform int texSlot;
uniform sampler2D texturethang[16];
uniform bool hasTex;

void main() {
    if (hasTex) {
        vec4 texColor = texture(texturethang[texSlot], vTexCoords.xy);
        if (texColor.a < 0.1)
            discard;
        FragColor = texColor;
    } else {
        FragColor = vec4(vColor, 1.f);
    }    
}