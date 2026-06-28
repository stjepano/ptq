#ifndef PTQ_COMMON_MEMORY_H_
#define PTQ_COMMON_MEMORY_H_

#include "com_types.h"

void *MemAlloc(u64 n);
void *MemRealloc(void *ptr, u64 n);
void MemFree(void *ptr);

void MemZero(void *ptr, u64 n);

#endif // PTQ_COMMON_MEMORY_H_
