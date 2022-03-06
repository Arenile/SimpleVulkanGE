#pragma once

#include "ge_pipeline.hpp"
#include "ge_device.hpp"
#include "ge_game_object.hpp"
#include "ge_camera.hpp"

#include <memory>
#include <vector>

#define VERT_SHADER_FILE_PATH       "shaders/simple_shader.vert.spv"
#define FRAG_SHADER_FILE_PATH       "shaders/simple_shader.frag.spv"

namespace ge 
{
    class GeRenderSystem
    {

        public:

        GeRenderSystem(GeDevice &device, VkRenderPass renderPass);
        ~GeRenderSystem();

        GeRenderSystem(const GeRenderSystem &) = delete;
        GeRenderSystem operator=(const GeRenderSystem &) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GeGameObject> &gameObjects, const GeCamera &camera);

        private:
            void createPipelineLayout();
            void createPipeline(VkRenderPass renderPass);

            GeDevice &geDevice;

            std::unique_ptr<GePipeline> gePipeline;
            VkPipelineLayout pipelineLayout;
    };
}