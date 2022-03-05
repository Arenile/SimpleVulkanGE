#pragma once

#include "ge_window.hpp"
#include "ge_device.hpp"
#include "ge_swap_chain.hpp"

#include <cassert>
#include <memory>
#include <vector>

#define VERT_SHADER_FILE_PATH       "shaders/simple_shader.vert.spv"
#define FRAG_SHADER_FILE_PATH       "shaders/simple_shader.frag.spv"

namespace ge 
{
    class GeRenderer
    {
        public:

        GeRenderer(GeWindow &window, GeDevice &device);
        ~GeRenderer();

        GeRenderer(const GeRenderer &) = delete;
        GeRenderer operator=(const GeRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return geSwapChain->getRenderPass(); }
        float getAspectRatio() const { return geSwapChain->extentAspectRatio(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress!");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const
        {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress!");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        private:
            void createCommandBuffers();
            void freeCommandBuffers();
            void recreateSwapChain();

            GeWindow& geWindow;
            GeDevice& geDevice;
            std::unique_ptr<GeSwapChain> geSwapChain;
            std::vector<VkCommandBuffer> commandBuffers;

            uint32_t currentImageIndex;
            int currentFrameIndex{0};
            bool isFrameStarted{false};
    };
}