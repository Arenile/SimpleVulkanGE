#include "ge_window.hpp"

#include <stdexcept>
#include <iostream>

namespace ge {

    GeWindow::GeWindow(int w, int h, std::string windowName) : 
        width{w}, height{h}, windowName{windowName} 
    {
            initWindow();
    }

    GeWindow::~GeWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void GeWindow::initWindow() 
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void GeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface!");
        }
    }

    void GeWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto geWindow = reinterpret_cast<GeWindow *>(glfwGetWindowUserPointer(window));
        geWindow->framebufferResized = true;
        geWindow->width = width;
        geWindow->height = height;
    }
}