project(Pong)

find_package(OpenGL)


add_executable(pong pong.c)
target_link_libraries(pong PRIVATE common portaudio)
target_include_directories(pong PRIVATE ../common)
