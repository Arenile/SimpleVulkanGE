#include "ge_window.hpp"

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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }
}