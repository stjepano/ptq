#include "com_surface.h"

#include <assert.h>

void SurfaceFillRect(surface_t *s, const irect_t *rect, u32 color)
{
    assert(s != nullptr && rect != nullptr);
    irect_t trect = {0};
    irect_t surface_rect = {0, 0, s->width, s->height};
    IRectIRectIntersection(&trect, rect, &surface_rect);

    if (trect.w <= 0 || trect.h <= 0)
    {
        return;
    }

    for (i32 row = 0; row < trect.h; row++)
    {
        for (i32 col = 0; col < trect.w; col++)
        {
            u32 *pixel =
                (u32 *)SurfacePixelPtr(s, col + trect.x, row + trect.y);
            *pixel = color;
        }
    }
}
