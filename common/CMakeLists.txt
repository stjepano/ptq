project(Common)

find_package(OpenGL)

add_library(common STATIC common.c platform.c memory.c surface.c)
target_link_libraries(common PUBLIC glfw)
target_link_libraries(common PRIVATE OpenGL::GL)
