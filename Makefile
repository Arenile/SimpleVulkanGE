CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

#create list of spv files for dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

GameEngine: *.cpp *.hpp
	g++ $(CFLAGS) -o GameEngine *.cpp $(LDFLAGS) && ./compile_shaders.sh

.PHONY: test clean

test: GameEngine
	./GameEngine

clean:
	rm -f GameEngine