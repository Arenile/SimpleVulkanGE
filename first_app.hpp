#pragma once

#include "ge_window.hpp"
#include "ge_pipeline.hpp"
#include "ge_device.hpp"
#include "ge_swap_chain.hpp"
#include "ge_model.hpp"

#include <memory>
#include <vector>

#define VERT_SHADER_FILE_PATH       "shaders/simple_shader.vert.spv"
#define FRAG_SHADER_FILE_PATH       "shaders/simple_shader.frag.spv"

namespace ge 
{
    class FirstApp
    {

        public:

        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp operator=(const FirstApp &) = delete;

        void run();

        private:
            void loadModels();
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void freeCommandBuffers();
            void drawFrame();
            void recreateSwapChain();
            void recordCommandBuffer(int imageIndex);

            void sierpinski(
                std::vector<GeModel::Vertex> &vertices,
                int depth,
                glm::vec2 left,
                glm::vec2 right,
                glm::vec2 top
            );

            GeWindow geWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
            GeDevice geDevice{geWindow};
            std::unique_ptr<GeSwapChain> geSwapChain;
            std::unique_ptr<GePipeline> gePipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
            std::unique_ptr<GeModel> geModel;
    };
}