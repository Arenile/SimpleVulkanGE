#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace ge
{
    class GeWindow
    {
        public:

            GeWindow(int w, int h, std::string windowName);
            ~GeWindow();

            //Delete C++ default copy constructor and operator to avoid unhappy accidents
            GeWindow(const GeWindow &) = delete;
            GeWindow &operator=(const GeWindow &) = delete;

            bool shouldClose() {return glfwWindowShouldClose(window);}

        private:

            void initWindow();

            const int width;
            const int height;
            std::string windowName;

            GLFWwindow *window;
    };
}