#ifndef PTQ_COMMON_SURFACE_H_
#define PTQ_COMMON_SURFACE_H_

#include "com_math.h"
#include "com_types.h"

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

#endif // PTQ_COMMON_SURFACE_H_
