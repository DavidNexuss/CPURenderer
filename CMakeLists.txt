cmake_minimum_required(VERSION 3.6)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(Draw CXX)
file(GLOB_RECURSE SRC src/*.cpp)
add_subdirectory(lib/glfw)
add_library(drawLib ${SRC})
target_link_libraries(drawLib glfw)

add_executable(client examples/client.cpp)
target_include_directories(client PUBLIC src)
target_link_libraries(client drawLib GL GLEW)


add_executable(demo examples/demo.cpp)
target_include_directories(demo PUBLIC src)
target_link_libraries(demo drawLib GL GLEW)
