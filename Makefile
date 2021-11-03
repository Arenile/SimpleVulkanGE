CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

GameEngine: *.cpp *.hpp
	g++ $(CFLAGS) -o GameEngine *.cpp $(LDFLAGS)

.PHONY: test clean

test: GameEngine
	./GameEngine

clean:
	rm -f GameEngine