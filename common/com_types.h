#ifndef PTQ_COMMON_TYPES_H_
#define PTQ_COMMON_TYPES_H_

#include <stdint.h>

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

#endif // PTQ_COMMON_TYPES_H_
