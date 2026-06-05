#pragma once
#include "GLFW/glfw3.h"
#include "PS_ErrorList.hpp"

namespace PistonSlap {
    class Window {
        public:

        Window() = default;
        ~Window();
        Window(const Window &) = delete;
        Window(Window &&) = delete;
        Window &operator=(const Window &) = delete;
        Window &operator=(Window &&) = delete;
        
        StatusCode initWindow(int width, int height, const char* header);
        [[nodiscard]] GLFWwindow* getWindow() const;
        [[nodiscard]] bool getShouldWindowClose() const;
        void markShouldWindowClose() const;
        
        inline static constexpr auto initWrangler = glfwInit;
        inline static constexpr auto pollEvents = glfwPollEvents;

        private:
        
        GLFWwindow *mpWindow{nullptr};
    };
};