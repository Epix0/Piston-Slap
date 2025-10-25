#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform float yScale;

out vec4 FragColor;
void main()
{
	vec4 finalPos = vec4(aPos.x, aPos.y * yScale, aPos.z, 1.0f);
		
	gl_Position = projection * view * model * finalPos;

	FragColor = (yScale == 0.0f) ? vec4(0.0f, 0.0f, 1.0f, 1.0f) : vec4(1.0f, 0.5f, 0.2f, 1.0f);
};