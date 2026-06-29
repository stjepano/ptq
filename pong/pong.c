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
    f32 phase;
    f32 freq;
} sine_wave_t;

sine_wave_t sine = {.phase = 0, .freq = 400};
sine_wave_t tone_a = {.phase = 0, .freq = 220};
sine_wave_t tone_b = {.phase = 0, .freq = 330};

static void sineWaveSamples(sine_wave_t *s, f32 *output, u32 frame_count)
{
    for (i32 i = 0; i < frame_count; i++)
    {
        f32 sample = sin(s->phase);
        s->phase += 2.0f * M_PI * (f32)s->freq / (f32)AUDIO_SAMPLE_RATE;
        if (s->phase > 2.0f * M_PI)
        {
            s->phase -= (2.0f * M_PI);
        }

        *output++ += sample;
        *output++ += sample;
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
    sineWaveSamples(&sine, output, frame_count);
    sineWaveSamples(&tone_a, output, frame_count);
    sineWaveSamples(&tone_b, output, frame_count);
    scaleSamples(output, frame_count);
}

#define FREQ_CHANGE_PER_SECOND 400.0
int main(int argc, char **argv)
{
    g_audio_scale = 0.25f;
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
    f64 t = t0;
    f64 dt = 0.0;
    ((void)dt);
    while (!glfwWindowShouldClose(g_window))
    {
        glfwPollEvents();

        sine.freq =
            400.0 + 200.0f * sin(2.0f * M_PI * 1.0f * (glfwGetTime() - t0));

        if (glfwGetKey(g_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(g_window, true);
        }

        fillSurface();

        PlatformPresent();

        glfwSwapBuffers(g_window);
        frame_count++;
        f64 tmpT = glfwGetTime();
        dt = tmpT - t;
        t = tmpT;
    }
    f64 delta_t = glfwGetTime() - t0;
    f64 fps = ((f64)frame_count) / delta_t;
    fprintf(stdout, "FPS: %.6f", fps);

    PlatformTerminate();
    return 0;
}
