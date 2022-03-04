#pragma once

#include "ge_window.hpp"
#include "ge_device.hpp"
#include "ge_game_object.hpp"
#include "ge_renderer.hpp"

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
            void loadGameObjects();

            GeWindow geWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
            GeDevice geDevice{geWindow};
            GeRenderer geRenderer{geWindow, geDevice};

            std::vector<GeGameObject> gameObjects;
    };
}