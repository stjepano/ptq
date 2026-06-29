#ifndef PTQ_COMMON_PLATFORM_H_
#define PTQ_COMMON_PLATFORM_H_

#include <GLFW/glfw3.h>
#include <portaudio.h>

#include "com_types.h"

#define AUDIO_SAMPLE_RATE 44100.0
typedef void (*audio_mixer_fn_t)(f32 *, u32, void *);

typedef struct
{
    i32 window_width;
    i32 window_height;
    const char *window_title;
    i32 surface_width;
    i32 surface_height;
    audio_mixer_fn_t audio_mixer_function;
} platform_config_t;

error_t PlatformInit(const platform_config_t *config);

void PlatformPresent();
void PlatformTerminate();

#endif // PTQ_COMMON_PLATFORM_H_
