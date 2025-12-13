#include <iostream>
#include "glm/glm.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <algorithm>
#include <cstdio>
#include "World.h"
#include <cstdlib>
#include <string>
#include <stb/stb_image.h>
#include <filesystem>
#include <map>
#include <memory>
#include "Instance.h"
#include "InstanceFactory.h"

// formula to viewmodel mathing
#if false
glm::mat4 armModel = glm::mat4(1.0f);
armModel = glm::translate(armModel, pCamera->Position);
armModel = glm::rotate(armModel, glm::radians(-pCamera->Yaw - 90.f), glm::vec3(0, 1, 0));
armModel = glm::rotate(armModel, glm::radians(pCamera->Pitch), glm::vec3(1, 0, 0));

// Viewmodel local offset
armModel = glm::translate(armModel, glm::vec3(0.3f, -0.3f, -0.5f));
armModel = glm::scale(armModel, glm::vec3(0.3f));

pArm->setMatrix(armModel);
#endif

// Instance Derived
#include "QuadPart.hpp"
#include "Character.hpp"

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
#include "CollisionSolver.h"
#include <vector>

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
std::shared_ptr<Camera> pCamera = nullptr;
bool SpacebearPressed = false;

//			Containers

std::vector<std::shared_ptr<Instance>> sessionInstances;

// Renderer stuff. If an Instance is to be rendered, add it here
std::vector<std::weak_ptr<Instance>> instancesToRender = {};

// Physics
std::vector<std::weak_ptr<Instance>> instancesToProcPhysics = {};
constexpr float CGravity_Strength = 0.04f;

//			/Containers

// Model stuff
auto& pImporter = CustomModelImporter::get();

// Timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float totalTime = 0.f;

// /GLOBAL

//	******* GLFW *******

GLFWwindow* createWindow();
GLFWmonitor* getMonitor();
void GLFW_mouse_callback(GLFWwindow* window, double xpos, double ypos);

//	******* GLFW *******

void mainMouseCallback(float xpos, float ypos, std::shared_ptr<Camera> pCamera);
void congifureWindow(GLFWwindow* window, GLFWmonitor* monitor);
void loadGLWrangler();
void configureGL();
void configureVendor();
void importModels();
// @rInputsMap will be populated
static void setupUserInput(std::map<int, Player::PlayerAction>& rInputsMap);
void grabInput(GLFWwindow* window, Player::PlayerPtr pPlayer);
void renderInstances(const std::vector<std::weak_ptr<Instance>>& instances, ShaderProgram& shader);
// Registers derived Instances with InstanceFactory. The desired Instances will be within this def
void registerInstances();
// Main physics proc func. This will definitely change later
void processPhysics(const std::vector<std::weak_ptr<Instance>>& instances);
template<typename instanceT> void addInstanceToWorld(std::string name); // I think type traits can help with having to specify template everytime

// GPT

inline std::pair<glm::vec3, glm::vec3>
computeWorldAABB(const glm::mat4& M, const glm::vec3& localMin, const glm::vec3& localMax) {
	// local center and half extents
	glm::vec3 localCenter = (localMin + localMax) * 0.5f;
	glm::vec3 halfSize = (localMax - localMin) * 0.5f;

	// world center
	glm::vec3 center = glm::vec3(M * glm::vec4(localCenter, 1.0f));

	// orientation / scale basis vectors
	glm::vec3 axisX = glm::vec3(M[0]);  // Right (scaled + rotated)
	glm::vec3 axisY = glm::vec3(M[1]);  // Up
	glm::vec3 axisZ = glm::vec3(M[2]);  // Forward

	// Abs dot products give projected half-size onto world axes
	float ex = std::abs(axisX.x) * halfSize.x +
		std::abs(axisY.x) * halfSize.y +
		std::abs(axisZ.x) * halfSize.z;

	float ey = std::abs(axisX.y) * halfSize.x +
		std::abs(axisY.y) * halfSize.y +
		std::abs(axisZ.y) * halfSize.z;

	float ez = std::abs(axisX.z) * halfSize.x +
		std::abs(axisY.z) * halfSize.y +
		std::abs(axisZ.z) * halfSize.z;

	glm::vec3 worldMin = center - glm::vec3(ex, ey, ez);
	glm::vec3 worldMax = center + glm::vec3(ex, ey, ez);

	return { worldMin, worldMax };
}

// GPT

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
	importModels();

	// Setup user input
	std::map<int, Player::PlayerAction> vInputActions;
	setupUserInput(vInputActions);

	//***		At this point, all assets are good for use		***//

	// Instances register
	auto& pInstanceFactory = InstanceFactory::get();
	registerInstances();	

	//*** Instances may now be used *** //

	{
		auto pBase = pInstanceFactory.cloneTemplate<QuadPart>("crate");
		if(pBase) {
			pBase->setPos(glm::vec3(0, -4.f, 0));
			pBase->setScale(glm::vec3(50.f, .5f, 50.f));
			instancesToRender.push_back(pBase);
			sessionInstances.push_back(pBase);
		}
	}

	// world
	auto pWorld = World::getWorld();

	// cam
	pCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), static_cast<float>(SCR_WIDTH) * .5f, static_cast<float>(SCR_HEIGHT) * .5f);

	// player
	pPlayer = std::make_shared<Player>(pCamera);
	pPlayer->setFlyingDetached(false);
	pPlayer->mHeight = -.4f;
	auto pCharacter = pInstanceFactory.cloneTemplate<QuadPart>("part");
	pCharacter->setScale(glm::vec3 (.1f, 0.5f, .1f));
	pPlayer->setCharacter(pCharacter);
	instancesToProcPhysics.push_back(pCharacter);
	sessionInstances.push_back(pCharacter);
	pCharacter->setFriendlyName("Epix0");
	pCharacter.reset();
	pCharacter = nullptr;
	//instancesToRender.push_back(pCharacter);

	// collision solver
	auto pCollisionSolver = std::make_unique<CollisionSolver>();

	//auto pTargetModel = pImporter->getModel("character");
	//auto& bounds = pTargetModel.lock()->getBounds();
	//pBox->setScale(glm::vec3((bounds.mMax - bounds.mMin)) * .5f);
#ifdef DEBUG // this bullshit's the skybox. Make this more neat before moving to Release plox
	// Tex
	auto cubemapTex = std::make_shared<Texture>(path("textures/skybox/"), GL_TEXTURE_CUBE_MAP);
	
	// Skybox Pos Vertices
	constexpr float skyboxVertices[] = {
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
	ShaderProgram opaqueLightingShader(Directory::Shaders + "OpaqueLighting.vert", Directory::Shaders + "JustAmbient.frag");
	ShaderProgram skyboxShader(Directory::Shaders + "Skybox.vert", Directory::Shaders + "Skybox.frag");
	
	srand(0);
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
		perspective = glm::perspective(glm::radians(90.0f), static_cast<float>(SCR_WIDTH) / std::max(static_cast<float>(SCR_HEIGHT), 1.0f), 0.001f, 200.0f);

		pPlayer->processInput();
		pPlayer->processCamera();

		opaqueLightingShader.use();
		opaqueLightingShader.setMat4("projection", perspective);
		opaqueLightingShader.setMat4("view", pCamera->GetViewMatrix());
		opaqueLightingShader.setVec3("viewPos", pCamera->Position);
		
		renderInstances(instancesToRender, opaqueLightingShader);
		processPhysics(instancesToProcPhysics);
		// Need a proper function to retrieve vertex positions transformed by Instance
		// After, need a proper method of finding the edge normal for 3D space

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

	auto spaceState = (glfwGetKey(window, GLFW_KEY_SPACE));
	if(spaceState == GLFW_PRESS && !SpacebearPressed) {
		addInstanceToWorld<QuadPart>("crate");
		SpacebearPressed = true;
	} else if(spaceState == GLFW_RELEASE)
		SpacebearPressed = false;

	for(auto& [key, action] : pPlayer->getKeybindsToActions()) {
		auto state = glfwGetKey(window, key);
		if(state == GLFW_PRESS || state == GLFW_REPEAT)
			pPlayer->pushAction(action);
		else {
			if(action == Player::PlayerAction::Forward || action == Player::PlayerAction::Backward)
				pPlayer->mPlayerMoveDirection.z = 0.f;
			if(action == Player::PlayerAction::Left || action == Player::PlayerAction::Right)
				pPlayer->mPlayerMoveDirection.x = 0.f;
		}
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

void registerInstances() {
	InstanceFactory& factory = InstanceFactory::get();
	auto& importer = CustomModelImporter::get();

	// || PART ||
	{
		auto part = std::make_unique<QuadPart>();
		part->setModel(importer.getModel(CustomModelImporter::NativeModelNames::Cube));
		part->setFriendlyName("part");
		part->setScale(.25f);
		factory.registerTemplate<QuadPart>("part", std::move(part));
	}

	// || CHARACTER ||
	factory.registerTemplate<Character>("character");

	// || CRATE ||
	{
		auto crate = std::make_unique<QuadPart>();
		crate->setModel(pImporter.getModel("crate"));
		factory.registerTemplate<QuadPart>("crate", std::move(crate));
	}
	// || ARM ||
	{
		auto arm = std::make_unique<QuadPart>();
		arm->setModel(importer.getModel("arm"));
		arm->setScale(.9f);
		factory.registerTemplate<QuadPart>("arm", std::move(arm));
	}
}

void processPhysics(const std::vector<std::weak_ptr<Instance>>& instances) {
	for(auto& wkpInstance : instances) {
		if(auto pInstance = wkpInstance.lock()) {
			if(pInstance->getAnchoredState())
				continue;

			auto currVel = pInstance->getVelocity();
			auto currPos = pInstance->getPos();

			glm::vec3 velOne = glm::vec3(0.f);
			velOne.y += currVel.y + -CGravity_Strength * deltaTime;

			if(pInstance == pPlayer->getCharacter()) {
				velOne += pPlayer->mPlayerMoveDirection * 3.f * deltaTime;
				if(pPlayer->mPlayerWantsToJump){
					if(velOne.y <= 1.f) {
						velOne.y += 0.25f * deltaTime;
					}

					pPlayer->mPlayerWantsToJump = false;
				}
			}

			if(currPos.y >= 50.f) {
				currPos.y = 49.f;
				velOne.y *= -1.f;
			}
			pInstance->setPos(currPos + velOne);

			/*TODO: 
				- skip Instances that lack a model, otherwise this GOOBs
				- implement a sleep state where objects at resting position but not anchored can be skipped during gravity comp.
					maybe a collision delta check?
			*/ 

			// Collision test
			if(sessionInstances.size() > 1) {
				for(int i = 0; i < sessionInstances.size(); ++i) {
					auto pInstanceToTest = sessionInstances.at(i);

					if(pInstanceToTest == pInstance) {
						if(currPos.y <= -50.f) {
							std::cout << "Destroying... " << pInstance->getFriendlyName() << "\n";
							sessionInstances.erase(sessionInstances.begin() + i);
						}
						continue;
					}

					auto [firstMinL, firstMaxL] = pInstance->getModel()->getBounds();
					auto [secondMinL, secondMaxL] = pInstanceToTest->getModel()->getBounds();
								
					glm::mat4 M1 = pInstance->getTransform();
					glm::mat4 M2 = pInstanceToTest->getTransform();

					auto [firstMinW, firstMaxW] =
						computeWorldAABB(M1, firstMinL, firstMaxL);

					auto [secondMinW, secondMaxW] =
						computeWorldAABB(M2, secondMinL, secondMaxL);
			
					bool overlap =
						firstMinW.x <= secondMaxW.x && firstMaxW.x >= secondMinW.x &&
						firstMinW.y <= secondMaxW.y && firstMaxW.y >= secondMinW.y &&
						firstMinW.z <= secondMaxW.z && firstMaxW.z >= secondMinW.z;

					if(overlap) {
						float overlapX = std::min(firstMaxW.x, secondMaxW.x) -
							std::max(firstMinW.x, secondMinW.x);
						float overlapY = std::min(firstMaxW.y, secondMaxW.y) -
							std::max(firstMinW.y, secondMinW.y);
						float overlapZ = std::min(firstMaxW.z, secondMaxW.z) -
							std::max(firstMinW.z, secondMinW.z);
											
						float minOverlap = overlapX;
						glm::vec3 normal(1, 0, 0);

						if(overlapY < minOverlap) {
							minOverlap = overlapY;
							normal = glm::vec3(0, 1, 0);
						}

						if(overlapZ < minOverlap) {
							minOverlap = overlapZ;
							normal = glm::vec3(0, 0, 1);
						}

						glm::vec3 centerA = (firstMinW + firstMaxW) * 0.5f;
						glm::vec3 centerB = (secondMinW + secondMaxW) * 0.5f;

						if(glm::dot(centerA - centerB, normal) < 0)
							normal = -normal;

						currPos += normal * minOverlap;
						velOne -= glm::dot(velOne, normal) * normal;

						pInstance->setPos(currPos);

						break;
					}
				}
			}

			pInstance->setVelocity(velOne);
		}
	}
}

template<typename instanceT> void addInstanceToWorld(std::string name) {
	auto pCharacter = pPlayer->getCharacter();
	if(!pCharacter) {
		return;
	}

	auto& factory = InstanceFactory::get();
	std::shared_ptr<Instance> pInstance = factory.cloneTemplate<instanceT>(std::move(name));
	pInstance->setScale(static_cast<float>(rand() % 10) * .01f);
	
	auto pos = pCharacter->getPos();
	pos.y -= pCharacter->getScale().y *2.f;
	pos.y -= pInstance->getScale().y*2.f;
	pInstance->setPos(pos);
	sessionInstances.push_back(pInstance);
	instancesToRender.push_back(pInstance);
	instancesToProcPhysics.push_back(pInstance);
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

void importModels() {
	auto& pImporter = CustomModelImporter::get();
	for(auto& modelDirEntry : std::filesystem::directory_iterator(Directory::Models)) {
		if(!modelDirEntry.is_directory())
			continue;
		
		// TODO: clone a blacklist of typical side-effect files that should not be imported but have to stay with the main model file
		for(auto& subModelDirEntry : std::filesystem::directory_iterator(modelDirEntry.path())) {
			if(subModelDirEntry.path().extension() == ".bin")
				continue;
			pImporter.ImportModelFile(subModelDirEntry.path());
		}
	}
}

void setupUserInput(std::map<int, Player::PlayerAction>& rInputsMap) {
	typedef Player::PlayerAction Action;
	rInputsMap[GLFW_KEY_W] = Action::Forward;
	rInputsMap[GLFW_KEY_S] = Action::Backward;
	rInputsMap[GLFW_KEY_A] = Action::Left;
	rInputsMap[GLFW_KEY_D] = Action::Right;
	rInputsMap[GLFW_KEY_F] = Action::Jump;
	rInputsMap[GLFW_KEY_LEFT_CONTROL] = Action::Crouch;
}