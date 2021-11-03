#pragma once

#include "ge_window.hpp"
#include "ge_pipeline.hpp"
#include "ge_device.hpp"

#define VERT_SHADER_FILE_PATH       "shaders/simple_shader.vert.spv"
#define FRAG_SHADER_FILE_PATH       "shaders/simple_shader.frag.spv"

namespace ge 
{
    class FirstApp
    {

        public:

        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();

        private:

            GeWindow geWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
            GeDevice geDevice{geWindow};
            GePipeline gePipeline{geDevice, 
                VERT_SHADER_FILE_PATH, 
                FRAG_SHADER_FILE_PATH, 
                GePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}