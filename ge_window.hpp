#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace ge
{
    class GeWindow
    {
        public:

            GLFWwindow* getWindow() {return window;}

            GeWindow(int w, int h, std::string windowName);
            ~GeWindow();

            //Delete C++ default copy constructor and operator to avoid unhappy accidents
            GeWindow(const GeWindow &) = delete;
            GeWindow &operator=(const GeWindow &) = delete;

            bool shouldClose() { return glfwWindowShouldClose(window); }
            bool wasWindowResized() { return framebufferResized; }
            void resetWindowResizedFlag() { framebufferResized = false; }
            VkExtent2D getExtent() { 
                return { static_cast<uint32_t>(width), 
                         static_cast<uint32_t>(height)}; }

            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
            GLFWwindow* getGLFWWindow() const { return window; }

        private:
            static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

            void initWindow();

            int width;
            int height;
            bool framebufferResized = false;

            std::string windowName;

            GLFWwindow *window;
    };
}