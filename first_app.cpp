#include "first_app.hpp"

namespace ge
{
    void FirstApp::run()
    {
        while (!geWindow.shouldClose())
        {
            glfwPollEvents();
        }
    }
}