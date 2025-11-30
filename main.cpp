#include "glm/glm.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stb/stb_image.h>
#include <filesystem>
#include <string>

// The default cube now has 2 meshes... but there are 3 VAOs. Perhaps that's why nothing's rendering. Start there
// In fact, there are weird numbers everywhere. Mesh vector is 8 in size when it should be 2???
using std::string, std::cout;

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

extern "C" {
}

// personal
#include "ShaderProgram.h"
#include "Camera.hpp"
#include "CustomIModelImporter.h"
#include <memory>
#include "Model.h"

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

static void grabModelInput(std::string& nameVar, std::shared_ptr<CustomModelImporter> pImporter);
static GLFWwindow* createWindow();
static void congifureWindow(GLFWwindow*& window);
static void loadGLWrangler();
static void configureGL();
static void configureVendor();
static void importModels(std::shared_ptr<CustomModelImporter> pImporter);

int main(int argsC, char* argsV[]) {
	GLFWwindow* window = createWindow();
	
	// Window hints and init config
	congifureWindow(window);
	
	// Essential for having defs for gl calls
	loadGLWrangler();
	
	// glEnable() calls and alike
	configureGL();

	// stuff like stb image
	configureVendor();

	// Import all models from models/
	auto pImporter = std::make_shared<CustomModelImporter>(); // main() shall be primary owner of this interface so that imported models remain valid
	importModels(pImporter);

	ShaderProgram shader(Directory::Shaders + "shader.vert", Directory::Shaders + "shader.frag");
	shader.use();

	Model& model = pImporter->getModel("character");

	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(.2f, 0, .5f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 perspective = glm::mat4(1.0f);
		perspective = glm::perspective(glm::radians(90.0f), static_cast<float>(SCR_WIDTH) / std::max(static_cast<float>(SCR_HEIGHT), 1.0f), 0.1f, 200.0f);

		shader.use();
		shader.setMat4("projection", perspective);
		shader.setMat4("view", camera.GetViewMatrix());
		shader.setVec3("viewPos", camera.Position);

		model.draw(shader);

		GLenum err = glGetError();
		if (err > 0)
			std::cout << err << "\n";

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	window = nullptr;
	std::cout << "Piston Slap Renderer closed successfully\n";
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

	// noclip mode
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	
	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS
}

void grabModelInput(std::string& nameVar, std::shared_ptr<CustomModelImporter> pImporter) {
	while(nameVar.empty()) {
		std::cout << "Enter a model name followed by the extension\n";
		std::cin >> nameVar;

		if(nameVar.empty())
			continue;

		if(!pImporter->ImportModelFile(Directory::Models + nameVar))
			nameVar.clear();
		else
			break;
	}
}

GLFWwindow* createWindow() {
	if(!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(640, 640, "PRERELEASE PistonSlap: Rendering", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	return window;
}

void congifureWindow(GLFWwindow*& window) {
	glfwSwapInterval(1);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void loadGLWrangler() {
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERR: Glad failed to init?\n";
	}

	gladLoadGL();
}

void configureGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void configureVendor() {
	stbi_set_flip_vertically_on_load(true);
}

void importModels(std::shared_ptr<CustomModelImporter> pImporter) {
	for(auto& modelDirEntry : std::filesystem::directory_iterator(Directory::Models)) {
		if(!modelDirEntry.is_directory())
			continue;
		
		// TODO: create a blacklist of typical side-effect files that should not be imported but have to stay with the main model file
		for(auto& subModelDirEntry : std::filesystem::directory_iterator(modelDirEntry.path())) {
			if(subModelDirEntry.path().extension() == ".bin")
				continue;
			pImporter->ImportModelFile(subModelDirEntry.path());
		}
	}
}
