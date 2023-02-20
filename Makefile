all: player main demo
player: player.cpp player.hpp util.hpp
	g++ player.cpp -o player -lGLEW -lGL -lglfw -O3 -DCLIENT
main: main.cpp player.hpp util.hpp
	g++ main.cpp -o main -O3 -fopenmp
demo: demo.cpp player.hpp util.hpp player.cpp
	g++ demo.cpp player.cpp -o demo -lGLEW -lGL -lglfw -O3
