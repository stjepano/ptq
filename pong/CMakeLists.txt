project(Pong)

find_package(OpenGL)


add_executable(pong pong.c)
target_link_libraries(pong PRIVATE common)
target_include_directories(pong PRIVATE ../common)
