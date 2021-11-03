#pragma once

#include "ge_window.hpp"
#include "ge_pipeline.hpp"

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
            GePipeline gePipeline{VERT_SHADER_FILE_PATH, FRAG_SHADER_FILE_PATH};
    };
}