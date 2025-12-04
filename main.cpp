#include <windows.h>
#include "glm/glm.hpp"
#include <algorithm>
#include <cstdio>
#include "World.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <stb/stb_image.h>
#include <filesystem>
#include <map>
#include <memory>
#include "Instance.h"

// something fishy going on when trying to load "character" model?

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
#include "Player.h"

// app settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 800;
const struct Directory {
	static inline const string Shaders = "shaders/";
	static inline const string Models = "models/";
	static inline const string Textures = "textures/";
};

// GLOBAL

// TODO: better client-launch setting. Global "pPlayer" feels wrong
//		PS -- this is here to gain scope to a camera during mouse callback. Get rid of this, then refactor mouse callback
std::shared_ptr<Player> pPlayer = nullptr;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float totalTime = 0.f;

// /GLOBAL

static GLFWwindow* createWindow();
GLFWmonitor* getMonitor();
// GLFW unique signature
void GLFW_mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mainMouseCallback(float xpos, float ypos, std::shared_ptr<Camera> pCamera);
static void congifureWindow(GLFWwindow* window, GLFWmonitor* monitor);
static void loadGLWrangler();
static void configureGL();
static void configureVendor();
static void importModels(std::shared_ptr<CustomModelImporter> pImporter);
// @rInputsMap will be populated
static void setupUserInput(std::map<int, Player::PlayerAction>& rInputsMap);
void grabInput(GLFWwindow* window, Player::PlayerPtr pPlayer);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void renderInstances(const std::vector<std::weak_ptr<Instance>>& instances, ShaderProgram& shader);

int main(int argsC, char* argsV[]) {
	GLFWwindow* window = createWindow();
	GLFWmonitor* monitor = getMonitor();

	// Window hints and init config
	congifureWindow(window, monitor);
	
	// Essential for having defs for gl calls
	loadGLWrangler();
	
	// glEnable() calls and alike
	configureGL();

	// stuff like stb image
	configureVendor();

	// Import all models from [models/*]. This implicitly imports [textures/*], too
	auto pImporter = std::make_shared<CustomModelImporter>(); // main() shall be primary owner of this interface so that imported models remain valid
	//importModels(pImporter);

	// Setup user input
	std::map<int, Player::PlayerAction> vInputActions;
	setupUserInput(vInputActions);

	// At this point, all assets are good for use

	// world
	auto pWorld = World::getWorld();

	// cam
	auto pCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), static_cast<float>(SCR_WIDTH) * .5f, static_cast<float>(SCR_HEIGHT) * .5f);

	// player
	pPlayer = std::make_shared<Player>(pCamera);
	pPlayer->mHeight = 2.f;
	pPlayer->setFlyingDetached(true);

	// instance
	auto pPlrCharacter = std::make_shared<Instance>("Epix0 Character", pImporter->getModel("character"));

	// combining
	pPlayer->setCharacter(pPlrCharacter);
	
	// renderer stuff
		// if an Instance is to be rendered, add it here
	std::vector<std::weak_ptr<Instance>> instancesToRender = { pPlrCharacter };

#ifdef DEBUG
	// Tex
	auto cubemapTex = std::make_shared<Texture>(path("textures/skybox/"), GL_TEXTURE_CUBE_MAP);

	// Skybox Pos Vertices
	const float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
#endif

//>>	Shaders		<< 
	
	// OpaqueLighting
	ShaderProgram opaqueLightingShader(Directory::Shaders + "OpaqueLighting.vert", Directory::Shaders + "OpaqueLighting.frag");
	ShaderProgram skyboxShader(Directory::Shaders + "Skybox.vert", Directory::Shaders + "Skybox.frag");

	while (!glfwWindowShouldClose(window))	{
		glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// world update
		pWorld->mDeltaTime = deltaTime;

		// this will for now handle player pos updates to the cam
		grabInput(window, pPlayer);
		
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(.2f, 0, .5f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 
		glm::mat4 perspective = glm::mat4(1.0f);
		perspective = glm::perspective(glm::radians(90.0f), static_cast<float>(SCR_WIDTH) / std::max(static_cast<float>(SCR_HEIGHT), 1.0f), 0.1f, 200.0f);

		pPlayer->processInput();
		pPlayer->processCamera();

		opaqueLightingShader.use();
		opaqueLightingShader.setMat4("projection", perspective);
		opaqueLightingShader.setMat4("view", pCamera->GetViewMatrix());
		opaqueLightingShader.setVec3("viewPos", pCamera->Position);

		renderInstances(instancesToRender, opaqueLightingShader);

#ifdef DEBUG
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		glm::mat4 view = glm::mat4(glm::mat3(pCamera->GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", perspective);
		skyboxShader.setInt("texturethang", cubemapTex->getAssignedTextureSlot());
		// skybox

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex->getGLTextureId());
		glBindVertexArray(skyboxVAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
		
#endif
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

void GLFW_mouse_callback(GLFWwindow*, double xposIn, double yposIn) {
	if(pPlayer) {
		mainMouseCallback(static_cast<float>(xposIn), static_cast<float>(yposIn), pPlayer->getCamera());
	}
}

void mainMouseCallback(float xpos, float ypos, std::shared_ptr<Camera> pCamera) {
	if(!pCamera) {
		return;
	}

	if(pCamera->mFirstMouse) {
		pCamera->mLastMouseX = xpos;
		pCamera->mLastMouseY = ypos;
		pCamera->mFirstMouse = false;
	}

	float xoffset = xpos - pCamera->mLastMouseX;
	float yoffset = pCamera->mLastMouseY - ypos; // reversed since y-coordinates go from bottom to top

	pCamera->mLastMouseX = xpos;
	pCamera->mLastMouseY = ypos;

	pCamera->ProcessMouseMovement(xoffset, yoffset);
}

void grabInput(GLFWwindow* window, Player::PlayerPtr pPlayer) {
	// TODO: refactor this bs
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}

	for(auto& [ key, action] : pPlayer->getKeybindsToActions()) {
		if(glfwGetKey(window, key) == GLFW_PRESS)
			pPlayer->pushAction(action);
	}

	//float yBob = sinf(lastFrame * 15.f) * 0.0008f;
	//float xBob = cosf(lastFrame * 15.f*.5) * -0.0008f;
	//camera.ProcessKeyboard(RIGHT, xBob);
	//camera.ProcessKeyboard(UP, yBob);
}

void renderInstances(const std::vector<std::weak_ptr<Instance>>& instances, ShaderProgram& shader) {
	for(auto& wkpInstance : instances) {
		if(auto pInstance = wkpInstance.lock()) {
			pInstance->draw(shader);
		}
	}
}

GLFWmonitor* getMonitor() {
	return glfwGetPrimaryMonitor();
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

void congifureWindow(GLFWwindow* window, GLFWmonitor* monitor) {
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSwapInterval(1);
	glfwSetCursorPosCallback(window, GLFW_mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#ifndef DEBUG
	glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	Sleep(500);
	glfwFocusWindow(window);
#endif
}

void loadGLWrangler() {
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERR: Glad failed to init?\n";
	}

	gladLoadGL();
}

void configureGL() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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

void setupUserInput(std::map<int, Player::PlayerAction>& rInputsMap) {
	typedef Player::PlayerAction Action;
	rInputsMap[GLFW_KEY_W] = Action::Forward;
	rInputsMap[GLFW_KEY_S] = Action::Backward;
	rInputsMap[GLFW_KEY_A] = Action::Left;
	rInputsMap[GLFW_KEY_D] = Action::Right;
	rInputsMap[GLFW_KEY_SPACE] = Action::Jump;
	rInputsMap[GLFW_KEY_LEFT_CONTROL] = Action::Crouch;
}