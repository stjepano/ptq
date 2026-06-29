project(Common)

find_package(OpenGL)

add_library(common STATIC common.c platform.c memory.c surface.c)
target_compile_options(common PRIVATE -Wall -Wpedantic -Werror)
target_link_libraries(common PUBLIC glfw miniaudio)
target_link_libraries(common PRIVATE OpenGL::GL)
