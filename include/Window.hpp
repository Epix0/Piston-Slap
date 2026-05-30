#pragma once
#include "GLFW/glfw3.h"

namespace PistonSlap {
    class Window {
    public:
        Window() : window(nullptr){};
        ~Window();
        Window(const Window &) = delete;
        Window(Window &&) = delete;
        Window &operator=(const Window &) = delete;
        Window &operator=(Window &&) = delete;

        void initWindow(int width, int height, const char* header);
        GLFWwindow* getWindow(){return window;};
    private:
        GLFWwindow *window;
    };
};