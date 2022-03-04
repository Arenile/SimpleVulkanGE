#pragma once

#include "ge_pipeline.hpp"
#include "ge_device.hpp"
#include "ge_game_object.hpp"

#include <memory>
#include <vector>

#define VERT_SHADER_FILE_PATH       "shaders/simple_shader.vert.spv"
#define FRAG_SHADER_FILE_PATH       "shaders/simple_shader.frag.spv"

namespace ge 
{
    class SimpleRenderSystem
    {

        public:

        SimpleRenderSystem(GeDevice &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem operator=(const SimpleRenderSystem &) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GeGameObject> &gameObjects);

        private:
            void createPipelineLayout();
            void createPipeline(VkRenderPass renderPass);

            GeDevice &geDevice;

            std::unique_ptr<GePipeline> gePipeline;
            VkPipelineLayout pipelineLayout;
    };
}