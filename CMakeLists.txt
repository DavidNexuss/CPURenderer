cmake_minimum_required(VERSION 3.6)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "Setting release build type")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(Draw CXX)
file(GLOB_RECURSE SRC src/*.cpp)
add_subdirectory(lib/glfw)
add_subdirectory(lib/stb)
add_library(drawLib ${SRC})
target_link_libraries(drawLib glfw stb)

add_executable(client examples/client.cpp)
target_include_directories(client PUBLIC src)
target_link_libraries(client drawLib GL GLEW)

add_executable(demo examples/demo.cpp)
target_include_directories(demo PUBLIC src)
target_link_libraries(demo drawLib GL GLEW)

add_executable(demo2 examples/demo2.cpp)
target_include_directories(demo2 PUBLIC src)
target_link_libraries(demo2 drawLib GL GLEW)


add_executable(multi examples/multi.cpp)
target_include_directories(multi PUBLIC src)
target_link_libraries(multi drawLib GL GLEW)

add_executable(input examples/input.cpp)
target_include_directories(input PUBLIC src)
target_link_libraries(input drawLib GL GLEW)


add_executable(texture examples/texture.cpp)
target_include_directories(texture PUBLIC src)
target_link_libraries(texture drawLib GL GLEW)
