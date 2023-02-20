cmake_minimum_required(VERSION 3.6)
project(stb)
file(GLOB SRC *.cpp)
add_library(stb ${SRC})
