#include "Window.hpp"
#include "PS_ErrorList.hpp"
#include "GLFW/glfw3.h"
#include "PS_Util.hpp"

namespace PistonSlap {
    void Window::initWindow(int width, int height, const char* header){
        GLFWwindow *newWindow = glfwCreateWindow(width, height, header, nullptr, nullptr);

        if (newWindow == nullptr){
            logStatus(StatusCode::GLFW_Window_Init_Failed, "initWindow() yieleded NULL");
        }
    }
}