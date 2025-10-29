// sys
#include <iostream>
#include <memory>

// vendor
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// personal
#include "ShaderProgram.h"
#include "InstanceService.h"
#include "PhysicsObject.h"
#include "CustomIModelImporter.h"

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main() {
	auto upImporter = std::make_unique<CustomModelImporter>();
	auto pImporter = upImporter.get();
	pImporter->ImportModelFile("mymodel.fbx");

	GLFWwindow* window;
	int width = 600;
	int height = 600;
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

	// This stuff will need wrapped soon!!!
	
	ShaderProgram shader("dependencies/shader.vert", "dependencies/shader.frag");
	shader.use();
	//VBO
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);

	// VAO
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(pImporter->vertices), pImporter->vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	//

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);
		glClearColor(.2f, 0, .5f, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glDrawArrays(GL_TRIANGLES, 0, 400);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}