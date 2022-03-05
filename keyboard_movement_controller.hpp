#pragma once

#include "ge_game_object.hpp"
#include "ge_window.hpp"


namespace ge
{

class KeyboardMovementController
{
    public:
        struct KeyMappings 
        {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveBackward = GLFW_KEY_S;
            int moveForward = GLFW_KEY_W;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookRight = GLFW_KEY_RIGHT;
            int lookLeft = GLFW_KEY_LEFT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;

        };

        void moveInPlaneXZ(GLFWwindow* window, float dt, GeGameObject &gameObject);

        KeyMappings keys{};
        float moveSpeed{3.f};
        float lookSpeed{1.5f};
    private:
};

}