#include "first_app.hpp"

#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace ge
{
    struct SimplePushConstantData 
    {
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    FirstApp::FirstApp()
    {
        loadGameObjects();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    FirstApp::~FirstApp()
    {
        vkDestroyPipelineLayout(geDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run()
    {
        while (!geWindow.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
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

    void FirstApp::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange;
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(geDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout)
            != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create Pipeline Layout!");
        }
    }

    void FirstApp::createPipeline()
    {
        assert(geSwapChain != nullptr && "Cannot create pipeline before swap chain!");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");
        
        PipelineConfigInfo pipelineConfig{};
        GePipeline::defaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.renderPass = geSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        gePipeline = std::make_unique<GePipeline>(
            geDevice,
            VERT_SHADER_FILE_PATH,
            FRAG_SHADER_FILE_PATH,
            pipelineConfig
        );
    }

    void FirstApp::recreateSwapChain()
    {
        auto extent = geWindow.getExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = geWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(geDevice.device());
        //geSwapChain = nullptr;
        if (geSwapChain == nullptr)
        {
            geSwapChain = std::make_unique<GeSwapChain>(geDevice, extent);
        }
        else 
        {
            geSwapChain = std::make_unique<GeSwapChain>(geDevice, extent, std::move(geSwapChain));
            if (geSwapChain->imageCount() != commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        //If render pass compatible no need to recreate like below
        createPipeline();
    }

    void FirstApp::createCommandBuffers()
    {

        commandBuffers.resize(geSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = geDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(geDevice.device(), &allocInfo, commandBuffers.data()) !=
        VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers!");
        }

    }

    void FirstApp::freeCommandBuffers()
    {
        vkFreeCommandBuffers(geDevice.device(), 
            geDevice.getCommandPool(), 
            static_cast<float>(commandBuffers.size()), 
            commandBuffers.data());
        
        commandBuffers.clear();
    }

    void FirstApp::recordCommandBuffer(int imageIndex)
    {

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = geSwapChain->getRenderPass();
        renderPassInfo.framebuffer = geSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = geSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.00f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(geSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(geSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, geSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        renderGameObjects(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer!");
        }
    }

    void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) 
    {
        gePipeline->bind(commandBuffer);

        for (auto& obj: gameObjects)
        {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.0001f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();

            vkCmdPushConstants(
                commandBuffer, 
                pipelineLayout, 
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
                0, 
                sizeof(SimplePushConstantData), 
                &push);

            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }

    void FirstApp::drawFrame()
    {

        uint32_t imageIndex;
        auto result = geSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        recordCommandBuffer(imageIndex);
        result = geSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        
        if (result == VK_ERROR_OUT_OF_DATE_KHR || 
            result == VK_SUBOPTIMAL_KHR ||
            geWindow.wasWindowResized())
        {
            geWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to present swap chain image");
        }
    }

    void FirstApp::sierpinski(
        std::vector<GeModel::Vertex> &vertices,
        int depth,
        glm::vec2 left,
        glm::vec2 right,
        glm::vec2 top)
    {
        if (depth <= 0)
        {
            vertices.push_back({top});
            vertices.push_back({right});
            vertices.push_back({left});
        }
        else
        {
            auto leftTop = 0.5f * (left+top);
            auto rightTop = 0.5f * (right+top);
            auto leftRight = 0.5f * (left+right);

            sierpinski(vertices, depth -1, left, leftRight, leftTop);
            sierpinski(vertices, depth -1, leftRight, right, rightTop);
            sierpinski(vertices, depth -1, leftTop, rightTop, top);
        }
    }
}