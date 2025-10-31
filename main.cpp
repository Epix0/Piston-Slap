// sys
#include <iostream>
#include <memory>

// vendor
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	window = glfwCreateWindow(640, 480, "Butt Monkey", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERR: Glad failed to init?\n";
		return 43;
	}

	// This stuff will need wrapped soon!!!
	
	ShaderProgram shader("dependencies/shader.vert", "dependencies/shader.frag");
	shader.use();

	// VAO
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//VBO
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pImporter->vertices.size() * sizeof(float), pImporter->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// EBO
	unsigned int EBO = 0;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pImporter->elementList.size() * sizeof(unsigned int), pImporter->elementList.data(), GL_STATIC_DRAW);
	
	//

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSwapInterval(1);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); // It's set in stone now: elements should be intended to render counter clockwise

	while (!glfwWindowShouldClose(window))
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, glm::radians(290.0f), glm::vec3(1.0f, 0.0f, 0.0));
		shader.setMat4("transform", trans);

		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);
		glClearColor(.2f, 0, .5f, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, pImporter->numOfVertices, GL_UNSIGNED_INT, 0);		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}