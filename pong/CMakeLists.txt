project(Pong)

find_package(OpenGL)


add_executable(pong pong.c ptq.c)
target_compile_options(pong PRIVATE -Wall -Wpedantic -Werror)
target_link_libraries(pong PRIVATE miniaudio glfw m OpenGL::GL)
