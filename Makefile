all: player main player.hpp util.hpp
player: player.cpp
	g++ player.cpp -o player -lGLEW -lGL -lglfw -O3
main: main.cpp
	g++ main.cpp -o main -O3 -fopenmp
