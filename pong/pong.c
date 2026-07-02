#include <assert.h>
#include <stdio.h>

#include <math.h>

#include "ptq.h"

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

#define UPDATE_FREQUENCY 60.0
#define UPDATE_PERIOD (1.0 / UPDATE_FREQUENCY)
#define UPDATE_ACCUMULATOR_CAP (3.0 * UPDATE_PERIOD)

static void genSineWaveSamples(f32 freq, f32 *output, u32 output_len)
{
    f32 phase = 0.0f;
    for (i32 i = 0; i < output_len; i++)
    {
        f32 sample = sin(phase);
        phase += 2.0f * M_PI * (f32)freq / (f32)AUDIO_SAMPLE_RATE;
        if (phase > 2.0f * M_PI)
        {
            phase -= (2.0f * M_PI);
        }

        *output++ = sample;
    }
}

int main(int argc, char **argv)
{
    platform_config_t conf = {.window_width = WINDOW_WIDTH,
                              .window_height = WINDOW_HEIGHT,
                              .window_title = WINDOW_TITLE,
                              .surface_width = SURFACE_WIDTH,
                              .surface_height = SURFACE_HEIGHT};
    error_t err = PlatformInit(&conf);
    if (err != ERROR_NO_ERROR)
    {
        fprintf(stderr, "Failed to initialize platform!\n");
        return 1;
    }

#define SAMPLE_COUNT 22050
    f32 sound_samples[SAMPLE_COUNT];
    genSineWaveSamples(330, sound_samples, SAMPLE_COUNT);

    sound_t sound = {.frame_count = SAMPLE_COUNT, .buffer = sound_samples};

    f64 update_accumulator = UPDATE_PERIOD;
    u64 update_count = 0;

    f64 t0 = glfwGetTime();
    f64 t1 = t0;
    u64 frame_count = 0;
    f64 delta_t_frame = 0.0;
    bool space_pressed_prev_frame = false;
    while (!glfwWindowShouldClose(g_window))
    {
        space_pressed_prev_frame =
            (glfwGetKey(g_window, GLFW_KEY_SPACE) == GLFW_PRESS);

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
            if (!space_pressed_prev_frame &&
                (glfwGetKey(g_window, GLFW_KEY_SPACE) == GLFW_PRESS))
            {
                PlaySound(&sound);
            }

            update_accumulator -= UPDATE_PERIOD;
            update_count++;
        }

        drawGradient();
        drawGreenBox();
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
