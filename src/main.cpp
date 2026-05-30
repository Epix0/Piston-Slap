#include <iostream>
#include <cstring>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

constexpr int ERR_NO_VULKAN_SUPPORT = 1;
constexpr int ERR_GLFW_INIT_FAILED = 2;
constexpr int ERR_GLFW_WINDOW_INIT_FAILED = 3;
constexpr int ERR_VULKAN_EXT_LOAD_ERR = 4;
constexpr int ERR_VULKAN_SURFACE_FAILED = 5;

// Custom error logger. Automatically newlines
void custom_err(int error, const std::string& desc){
    std::cout << "ERROR [" << error << "] " << desc << "\n";
}

// GLFW-specific error logger
void error_callback(int error, const char* description){
    std::cout << "GLFW RUNETIME ERR: " << description << "\n";
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
}

void GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
 
int main(){
    GLFWwindow *window = nullptr;

    glfwSetErrorCallback(error_callback);

    if (glfwInit() == 0){
        custom_err(ERR_GLFW_INIT_FAILED, "glfw could not initialize");
        return 1;
    }

    if (glfwVulkanSupported() == 0){
        custom_err(ERR_NO_VULKAN_SUPPORT, "glfw could not identify vulkan support");
        return ERR_NO_VULKAN_SUPPORT;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // ensure no GL context
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(800, 800, "Piston-Slap Renderer", nullptr, nullptr);

    if (window == nullptr){
        glfwTerminate();
        return ERR_GLFW_WINDOW_INIT_FAILED;
    }

    // VK SETUP DONE //
    glfwSetKeyCallback(window, GLFW_KeyCallback);

    while (!glfwWindowShouldClose(window)){

        glfwPollEvents();
    }

   
    glfwTerminate();
    std::cout << "Piston-Slap terminated successfully\n";
    return 0;
}