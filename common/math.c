#include "com_math.h"

void IRectIRectIntersection(irect_t *out, const irect_t *a, const irect_t *b)
{
    out->x = MAX(a->x, b->x);
    out->y = MAX(a->y, b->y);
    out->w = MIN(a->x + a->w, b->x + b->w) - out->x;
    out->h = MIN(a->y + a->h, b->y + b->h) - out->y;
}
