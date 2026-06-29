#include <assert.h>
#include <stdio.h>

#include "com_memory.h"
#include "com_platform.h"
#include "com_surface.h"

#include <math.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_TITLE "Pong"

#define SURFACE_WIDTH 960
#define SURFACE_HEIGHT 540

extern GLFWwindow *g_window;
extern surface_t *g_surface;
extern f32 g_audio_scale;

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

typedef struct
{
    f32 freq;
    f32 time;
} sine_wave_t;

sine_wave_t sine_a = {.freq = 330.0f, .time = 0.0f};
sine_wave_t sine_b = {.freq = 150.0f, .time = 0.0f};
sine_wave_t sine_c = {.freq = 440.0f, .time = 0.0f};

static void sineWaveSamples(sine_wave_t *sine_wave, f32 *output,
                            u32 frame_count)
{
    const f32 deltaT = 1.0f / (f64)AUDIO_SAMPLE_RATE;
    for (i32 i = 0; i < frame_count; i++)
    {
        sine_wave->time += deltaT;
        f32 val = sin(2.0f * M_PI * sine_wave->freq * sine_wave->time);
        *output++ += val;
        *output++ += val;
    }
}

static void scaleSamples(f32 *output, u32 frame_count)
{
    for (i32 i = 0; i < frame_count; i++)
    {
        *output++ *= g_audio_scale;
        *output++ *= g_audio_scale;
    }
}

static void audioMixer(f32 *output, u32 frame_count, void *user_data)
{
    ((void)user_data);
    MemZero(output, sizeof(f32) * frame_count * 2);
    sineWaveSamples(&sine_b, output, frame_count);
    sineWaveSamples(&sine_a, output, frame_count);
    sineWaveSamples(&sine_c, output, frame_count);
    scaleSamples(output, frame_count);
}

int main(int argc, char **argv)
{
    g_audio_scale = 0.05f;
    platform_config_t conf = {.window_width = WINDOW_WIDTH,
                              .window_height = WINDOW_HEIGHT,
                              .window_title = WINDOW_TITLE,
                              .surface_width = SURFACE_WIDTH,
                              .surface_height = SURFACE_HEIGHT,
                              .audio_mixer_function = audioMixer};
    error_t err = PlatformInit(&conf);
    if (err != ERROR_NO_ERROR)
    {
        fprintf(stderr, "Failed to initialize platform!\n");
        return 1;
    }

    f64 t0 = glfwGetTime();
    u64 frame_count = 0;
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
