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

#define OFFSET_SPEED 500.0f
static f32 s_x_offset = 0;
static f32 s_y_offset = 0;

static void drawGradient()
{
    for (i32 row = 0; row < g_surface->height; row++)
    {
        for (i32 col = 0; col < g_surface->width; col++)
        {
            u32 *ptr = SurfacePixelPtr(g_surface, col, row);

            u32 red = (((u32)(col + s_x_offset) >> 5) << 5) % 256;
            u32 blue = (((u32)(row + s_y_offset) >> 5) << 5) % 256;

            u32 color = EncodeRGBA(red, 0, blue, 0xff);
            *ptr = color;
        }
    }
}

static void drawGreenBox()
{

    irect_t rect = {10, 10, 50, 30};
    SurfaceFillRect(g_surface, &rect, 0xFF00FF00);
}

typedef struct
{
    f32 phase;
    f32 freq;
} sine_wave_t;

sine_wave_t mid_c_tone = {.phase = 0, .freq = 260};
sine_wave_t mid_e_tone = {.phase = 0, .freq = 400};

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
    sineWaveSamples(&mid_c_tone, output, frame_count);
    sineWaveSamples(&mid_e_tone, output, frame_count);
    scaleSamples(output, frame_count);
}

#define UPDATE_FREQUENCY 60.0
#define UPDATE_PERIOD (1.0 / UPDATE_FREQUENCY)
#define UPDATE_ACCUMULATOR_CAP (3.0 * UPDATE_PERIOD)

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

    f64 update_accumulator = UPDATE_PERIOD;
    u64 update_count = 0;

    f64 t0 = glfwGetTime();
    f64 t1 = t0;
    u64 frame_count = 0;
    f64 delta_t_frame = 0.0;
    while (!glfwWindowShouldClose(g_window))
    {
        glfwPollEvents();

        update_accumulator += delta_t_frame;
        update_accumulator = MIN(update_accumulator, UPDATE_ACCUMULATOR_CAP);
        while (update_accumulator >= UPDATE_PERIOD)
        {
            if (glfwGetKey(g_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(g_window, true);
            }
            if (glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                s_y_offset -= (OFFSET_SPEED * UPDATE_PERIOD);
            }
            if (glfwGetKey(g_window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                s_y_offset += (OFFSET_SPEED * UPDATE_PERIOD);
            }
            if (glfwGetKey(g_window, GLFW_KEY_LEFT) == GLFW_PRESS)
            {
                s_x_offset -= (OFFSET_SPEED * UPDATE_PERIOD);
            }
            if (glfwGetKey(g_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            {
                s_x_offset += (OFFSET_SPEED * UPDATE_PERIOD);
            }

            drawGradient();
            drawGreenBox();

            update_accumulator -= UPDATE_PERIOD;
            update_count++;
        }

        PlatformPresent();
        frame_count++;
        f64 tmpT = glfwGetTime();
        delta_t_frame = tmpT - t1;
        t1 = tmpT;
    }
    f64 loop_time = glfwGetTime() - t0;
    f64 fps = ((f64)frame_count) / loop_time;
    f64 ups = ((f64)update_count) / loop_time;
    fprintf(stdout, "FPS: %.6f, UPS: %.6f\n", fps, ups);

    PlatformTerminate();
    return 0;
}
