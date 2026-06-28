#include <assert.h>
#include <stdio.h>

#include "com_platform.h"
#include "com_surface.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_TITLE "Pong"

#define SURFACE_WIDTH 960
#define SURFACE_HEIGHT 540

extern GLFWwindow *g_window;
extern surface_t *g_surface;

static void fillSurface()
{
    for (i32 row = 0; row < g_surface->height; row++)
    {
        for (i32 col = 0; col < g_surface->width; col++)
        {
            u8 *ptr = g_surface->pixels +
                      ((row * g_surface->width * g_surface->format) +
                       (col * g_surface->format));
            u32 red = ((col >> 5) << 5) % 256;
            u32 blue = ((row >> 5) << 5) % 256;

            u32 color = 0xFF000000;
            color |= blue << 16;
            color |= red;

            *((u32 *)ptr) = color;
        }
    }
}

int main(int argc, char **argv)
{
    error_t err = PlatformInit(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE,
                               SURFACE_WIDTH, SURFACE_HEIGHT);
    if (err != ERROR_NO_ERROR)
    {
        fprintf(stderr, "Failed to initialize platform!\n");
        return 1;
    }

    GLfloat clear_color[] = {0.12f, 0.13f, 0.11f, 1.0f};

    f64 t0 = glfwGetTime();
    u64 frame_count = 0;
    int framebuffer[2] = {0};
    while (!glfwWindowShouldClose(g_window))
    {
        glfwPollEvents();
        if (glfwGetKey(g_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(g_window, true);
        }

        fillSurface();

        PlatformPresent();

        glfwSwapBuffers(g_window);
        frame_count++;
    }
    f64 delta_t = glfwGetTime() - t0;
    f64 fps = ((f64)frame_count) / delta_t;
    fprintf(stdout, "FPS: %.6f", fps);

    PlatformTerminate();
    return 0;
}
