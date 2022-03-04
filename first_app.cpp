#include "first_app.hpp"
#include "simple_render_system.hpp"

#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace ge
{

    FirstApp::FirstApp()
    {
        loadGameObjects();
    }

    FirstApp::~FirstApp()
    {

    }

    void FirstApp::run()
    {
        SimpleRenderSystem simpleRenderSystem{geDevice, geRenderer.getSwapChainRenderPass()};

        while (!geWindow.shouldClose())
        {
            glfwPollEvents();
            if (auto commandBuffer = geRenderer.beginFrame())
            {
                // Example of future stuff we might do...
                // begin offscreen shadow pass
                // render shadow casting objects
                // end offscreen shadow pass 

                geRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                geRenderer.endSwapChainRenderPass(commandBuffer);
                geRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(geDevice.device());
    }

    void FirstApp::loadGameObjects()
    {
        std::vector<GeModel::Vertex> vertices {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
/*
 *       std::vector<GeModel::Vertex> vertices {};
 *       sierpinski(vertices, 7, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
 */     
        auto geModel = std::make_shared<GeModel>(geDevice, vertices);

        auto triangle = GeGameObject::createGameObject();
        triangle.model = geModel;
        triangle.color = {0.5f, 0.1f, 0.6f};
        triangle.transform2d.translation.x = 0.2f;
        triangle.transform2d.scale = {2.0f, 0.5f};
        triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }
}