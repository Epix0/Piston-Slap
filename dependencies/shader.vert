#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 transform;
uniform mat4 perspective;
uniform mat4 camera;
uniform vec3 aLightPos;

out vec3 normal;
out vec3 fragPos;
out vec3 lightPos;

void main()
{
	//vec4 finalPos = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	//gl_Position = vec4(aPos, 1.0f);
	gl_Position = perspective * transform * camera * vec4(aPos, 1.0);

	lightPos = aLightPos;
	fragPos = aPos;
	normal = aNormal;
};