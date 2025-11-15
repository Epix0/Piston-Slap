#version 330 core
in vec3 normal;
in vec3 fragPos;
out vec4 FinalFragColor;
in vec3 lightPos;

void main()
{
	//vec3 lightPos = vec3(0, 5.0f, 0);
	//vec3 normalNormalized = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * vec3(1.0f);

	vec3 result = (vec3(0.3f, .3f, 0) + diffuse) * vec3(0, 0.5f, 0);
	FinalFragColor = vec4(result, 1.0f);
};