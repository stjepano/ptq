#ifndef PTQ_COMMON_SURFACE_H_
#define PTQ_COMMON_SURFACE_H_

#include "com_types.h"

typedef enum
{
    SurfaceFormat_R = 1,
    SurfaceFormat_RGB = 3,
    SurfaceFormat_RGBA = 4
} surface_format_t;

typedef struct
{
    i32 width;
    i32 height;
    surface_format_t format;
    u8 *pixels;
} surface_t;

#endif // PTQ_COMMON_SURFACE_H_
