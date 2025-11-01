#version 330 core
in vec3 normal;
in vec3 fragPos;
out vec4 FinalFragColor;
void main()
{
	vec3 lightPos = vec3(.1f, .8f, 8);
	vec3 normalNormalized = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normalNormalized, lightDir), 0.0f);
	vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);

	vec3 result = (vec3(.5f, .5f, .5f) + diffuse) * vec3(.3f, .3, 0.6f);
	FinalFragColor = vec4(result, 1.0f);
};