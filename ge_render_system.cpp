#include "ge_render_system.hpp"

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
        glm::mat4 transform{1.f};
        alignas(16) glm::vec3 color;
    };

    GeRenderSystem::GeRenderSystem(GeDevice &device, VkRenderPass renderPass) : geDevice{device} 
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    GeRenderSystem::~GeRenderSystem()
    {
        vkDestroyPipelineLayout(geDevice.device(), pipelineLayout, nullptr);
    }

    void GeRenderSystem::createPipelineLayout()
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

    void GeRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");
        
        PipelineConfigInfo pipelineConfig{};
        GePipeline::defaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        gePipeline = std::make_unique<GePipeline>(
            geDevice,
            VERT_SHADER_FILE_PATH,
            FRAG_SHADER_FILE_PATH,
            pipelineConfig
        );
    }

    void GeRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GeGameObject> &gameObjects, const GeCamera &camera) 
    {
        gePipeline->bind(commandBuffer);

        auto projectionView = camera.getProjection() * camera.getView();

        for (auto& obj: gameObjects)
        {
            SimplePushConstantData push{};
            push.color = obj.color;
            push.transform = projectionView * obj.transform.mat4();

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
}