#include "ge_renderer.hpp"

#include <stdexcept>
#include <array>

namespace ge
{
    GeRenderer::GeRenderer(GeWindow &window, GeDevice &device) : geWindow{window}, geDevice{device}
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    GeRenderer::~GeRenderer()
    {
        freeCommandBuffers();
    }

    void GeRenderer::recreateSwapChain()
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
            std::shared_ptr<GeSwapChain> oldSwapChain = std::move(geSwapChain);
            geSwapChain = std::make_unique<GeSwapChain>(geDevice, extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*geSwapChain.get()))
            {
                throw std::runtime_error("Swap chain image format has changed!");
            }
        }
    }

    void GeRenderer::createCommandBuffers()
    {

        commandBuffers.resize(GeSwapChain::MAX_FRAMES_IN_FLIGHT);

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

    void GeRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(geDevice.device(), 
            geDevice.getCommandPool(), 
            static_cast<float>(commandBuffers.size()), 
            commandBuffers.data());
        
        commandBuffers.clear();
    }

    VkCommandBuffer GeRenderer::beginFrame()
    {
        assert(!isFrameStarted && "Can't call beginFrame() while a frame is already in progress!");

        auto result = geSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        return commandBuffer;
    }
    void GeRenderer::endFrame() 
    {
        assert(isFrameStarted && "Can't call endFrame() while frame is not in progress!");

        auto commandBuffer = getCurrentCommandBuffer();

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer!");
        }

        auto result = geSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        
        if (result == VK_ERROR_OUT_OF_DATE_KHR || 
            result == VK_SUBOPTIMAL_KHR ||
            geWindow.wasWindowResized())
        {
            geWindow.resetWindowResizedFlag();
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to present swap chain image");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % GeSwapChain::MAX_FRAMES_IN_FLIGHT;
    }
    void GeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass() if frame is not in progress!");
        assert(commandBuffer == getCurrentCommandBuffer() && 
            "Can't begin render pass on command buffer from a different frame!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = geSwapChain->getRenderPass();
        renderPassInfo.framebuffer = geSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = geSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.00f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(geSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(geSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, geSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    }
    void GeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass() if frame is not in progress!");
        assert(commandBuffer == getCurrentCommandBuffer() && 
            "Can't end render pass on command buffer from a different frame!");

        vkCmdEndRenderPass(commandBuffer);
    }

}