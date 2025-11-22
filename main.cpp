// sys
#include <iostream>
#include <memory>
#include <string>

using std::string;

// vendor
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION // [1])
#include <stb/stb_image.h>		//	[2])

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// personal
#include "ShaderProgram.h"
#include "CustomIModelImporter.h"
#include "Camera.hpp"

// app settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 800;
const struct Directory {
	static inline const string Shaders = "shaders/";
	static inline const string Models = "models/";
	static inline const string Textures = "textures/";
};

// GLOBAL

// main world cam
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos(0);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// /GLOBAL

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(640, 480, "PRERELEASE PistonSlap: Rendering", NULL, NULL);
	
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERR: Glad failed to init?\n";
		return 43;
	}

	// This stuff will need wrapped soon!!!
	
	ShaderProgram shader(Directory::Shaders + "shader.vert", Directory::Shaders + "shader.frag");
	shader.use();

	auto pImporter = std::make_unique<CustomModelImporter>();
	pImporter->ImportModelFile(Directory::Models + "test.stl");

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);

	glm::vec3 worldUp = glm::vec3(0, 1.0f, 0.0f);
	
	//stb bs
	//int width, height, nrChannels;
	//unsigned char* data = stbi_load("dependencies/textures/container.jpg", &width, &height, &nrChannels, 0);
	//unsigned int texture;
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_3D, texture);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	////glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, width, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_3D);
	//stbi_image_free(data);

	Model& myModel = pImporter->mImportedModels[0];

	myModel.setPos(glm::vec3(0));

	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		
		glm::mat4 perspective = glm::mat4(1.0f);
		perspective = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 10.0f);

		//myModel.setPos(glm::vec3(0, (sinf(currentFrame * 6.0f) * 20.0f) * deltaTime, 0));
		//myModel.setOrientationDeg(glm::vec3(0, currentFrame * 60.0f, 0));
		//myModel.updateMatrix();

		shader.use();
		shader.setMat4("projection", perspective);
		shader.setMat4("view", camera.GetViewMatrix());
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", lightPos);
		shader.setVec3("objectColor", glm::vec3(1.0f, 0, 0));
		shader.setVec3("lightColor", glm::vec3(1.0f));

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(.2f, 0, .5f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myModel.Draw(shader);
	
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDrawElements(GL_TRIANGLES, pMesh->elementList.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		lightPos = camera.Position;
}