#pragma once

#include "ge_device.hpp"

#include <string>
#include <vector>

namespace ge
{
    struct PipelineConfigInfo {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class GePipeline
    {
        public:
            GePipeline(
                GeDevice &device, 
                const std::string& vertFilepath, 
                const std::string& fragFilepath, 
                const PipelineConfigInfo &pipeConfigInfo
            );
            
            ~GePipeline();

            GePipeline() = default;
            GePipeline(const GePipeline&) = delete;
            GePipeline& operator=(const GePipeline&) = delete;

            void bind(VkCommandBuffer commandBuffer);
            
            static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

        private:
            static std::vector<char> readFile(const std::string& filepath);

            void createGraphicsPipeline(
                const std::string& vertFilepath, 
                const std::string& fragFilepath,
                const PipelineConfigInfo &pipeConfigInfo
            );
            
            void createShaderModule(
                const std::vector<char>& code,
                VkShaderModule* shaderModule
            );

            GeDevice& geDevice;
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
    };
}