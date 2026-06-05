#include "PS_Window.hpp"

#include <vulkan/vulkan.hpp>
#include "GLFW/glfw3.h"
#include "PS_ErrorList.hpp"
#include "PS_Util.hpp"

void GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

namespace PistonSlap {
    Window::~Window() {
        if (nullptr == mpWindow){
            return;
        }

        glfwDestroyWindow(mpWindow);
        glfwTerminate();
    };

    // Move the logic for key callbacks
    StatusCode Window::initWindow(int width, int height, const char* header){
       GLFWwindow *newWindow = glfwCreateWindow(width, height, header, nullptr, nullptr);

        if (newWindow == nullptr){
            logStatus(PistonSlap::StatusCode::GLFW_Window_Init_Failed, "initWindow() yieleded NULL");
            return StatusCode::GLFW_Window_Init_Failed;
        };
        
        mpWindow = newWindow;
        glfwMakeContextCurrent(newWindow);
        glfwSetKeyCallback(newWindow, GLFW_KeyCallback);
        return StatusCode::OK;
    };

    StatusCode initWrangler() {
        if (0 != glfwInit()){
            logStatus(StatusCode::GLFW_MAIN_INIT_FAILED, "Failed during initWrangler");
            return StatusCode::GLFW_MAIN_INIT_FAILED;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        return StatusCode::OK;
    };

    bool Window::getShouldWindowClose() const {
        if (mpWindow == nullptr){
            return true;
        }

        return glfwWindowShouldClose(mpWindow);
    };

    void Window::markShouldWindowClose() const{
        glfwSetWindowShouldClose(mpWindow, GL_TRUE);
    };


}