#version 330 core
out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;  
in vec2 vTexCoords;
in vec3 vColor;

uniform int texSlot;
uniform sampler2D texturethang[5];
uniform bool hasTex;
uniform vec3 viewPos; // cam pos

void main() {
    // CONFIGS

    // Global
    vec3 norm = normalize(vNormal);

    // Diffuse related
    const vec3 lightPos = vec3(1.f); // convert to uniform later
    const vec3 lightColor = vec3(1.f); // convert to uniform later
    const float ambientStrength = 0.5f; // strength of raw object color without any diffuse light on it

    // Specular related
    const float specularStrength = 0.8f;

    // end CONFIGS

    // Diffuse calc
    vec3 ambient = ambientStrength * lightColor;
    vec3 lightDir = normalize(vec3(0.f, 1.f, 0.f));//normalize(lightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = diff * lightColor;

    vec3 camDir = normalize(viewPos - vFragPos);

    // Specular calc
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-camDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), 2);
    vec3 specular = specularStrength * spec * lightColor; 

    // Point light calc
    float constant = 1.f;
    float linear = 0.09f;
    float quadratic =  0.032f;
    float distanceFromSrc = length(lightPos - vFragPos);
    float attenuation = 1.0 / (constant + linear * distanceFromSrc + 
    		    quadratic * (distanceFromSrc * distanceFromSrc));

    // Stage: modify final light values with Point light attenuation
    if (false){
        ambient  *= attenuation; 
        diffuse  *= attenuation;
        specular *= attenuation;
    }
    

    if (hasTex) {
        vec4 texColor = texture(texturethang[texSlot], vTexCoords.xy);
        if (texColor.a < 0.1)
            discard;
        FragColor = texColor;
    } else {
        FragColor = vec4((ambient + specular) * vColor, 1.f);
    }    
}