#include "com_memory.h"

#include <stdlib.h>
#include <string.h>

void *MemAlloc(u64 n)
{
    return malloc(n);
}

void *MemRealloc(void *ptr, u64 n)
{
    return realloc(ptr, n);
}

void MemFree(void *ptr)
{
    free(ptr);
}

void MemZero(void *ptr, u64 n)
{
    memset(ptr, 0, n);
}
