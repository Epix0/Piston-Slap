#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transform;

out vec4 FragColor;
void main()
{
	//vec4 finalPos = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
		
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);

	FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);
};