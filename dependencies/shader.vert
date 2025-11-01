#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 transform;

out vec3 normal;
out vec3 fragPos;

void main()
{
	//vec4 finalPos = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
		
	gl_Position = transform * vec4(aPos, 1.0);

	fragPos = aPos;
	normal = aNormal;
};