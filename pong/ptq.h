#ifndef PTQ_H_
#define PTQ_H_

#include <GLFW/glfw3.h>
#include <miniaudio.h>
#include <stdint.h>

// Types

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef i32 error_t;
#define ERROR_NO_ERROR 0
#define ERROR_FAILED 1

// Utils
#define ASSERT(x) assert(x)
#define UNUSED(x) ((void)x)

#define DEBUG_PRINT_I32(var) printf(#var ": %d\n", var)
#define DEBUG_PRINT_F32(var) printf(#var ": %.6f\n", var)

// Memory

void *MemAlloc(u64 n);
void *MemRealloc(void *ptr, u64 n);
void MemFree(void *ptr);
void MemZero(void *ptr, u64 n);

// Math

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct
{
    i32 x, y, w, h;
} irect_t;

void IRectIRectIntersection(irect_t *out, const irect_t *a, const irect_t *b);

// Platform

#define AUDIO_SAMPLE_RATE 44100.0

typedef struct
{
    i32 window_width;
    i32 window_height;
    const char *window_title;
    i32 surface_width;
    i32 surface_height;
} platform_config_t;

error_t PlatformInit(const platform_config_t *config);

void PlatformPresent();
void PlatformTerminate();

// Surface

typedef struct
{
    i32 width;
    i32 height;
    u32 *pixels;
} surface_t;

#define EncodeRGBA(r, g, b, a)                                                 \
    ((((a) & 0xff) << 24) | (((b) & 0xff) << 16) | (((g) & 0xff) << 8) |       \
     ((r) & 0xff))

#define SurfacePixelPtr(sptr, x, y)                                            \
    ((sptr)->pixels + (((y) * (sptr)->width)) + (x))

void SurfaceFillRect(surface_t *s, const irect_t *rect, u32 color);

// Sound

typedef struct
{
    i32 frame_count;
    f32 *buffer;
} sound_t;

void PlaySound(sound_t *snd);

#endif // PTQ_H_
