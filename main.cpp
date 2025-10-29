// sys
#include <iostream>
#include <memory>

// vendor
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// personal
#include "InstanceService.h"
#include "PhysicsObject.h"

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main() {
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

		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}