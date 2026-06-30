#ifndef PTQ_COMMON_MATH_H_
#define PTQ_COMMON_MATH_H_

#include "com_types.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct
{
    i32 x, y, w, h;
} irect_t;

void IRectIRectIntersection(irect_t *out, const irect_t *a, const irect_t *b);

#endif // PTQ_COMMON_MATH_H_
