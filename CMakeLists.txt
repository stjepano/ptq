cmake_minimum_required(VERSION 3.20)
project(PTQ)

set(CMAKE_C_STANDARD 23)

add_subdirectory(./deps/glfw)
# add_subdirectory(./deps/portaudio)
add_subdirectory(./deps/miniaudio)
add_subdirectory(./common)
add_subdirectory(./pong)
