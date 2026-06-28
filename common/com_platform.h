#ifndef PTQ_COMMON_PLATFORM_H_
#define PTQ_COMMON_PLATFORM_H_

#include <GLFW/glfw3.h>

#include "com_types.h"

error_t PlatformInit(i32 framebuffer_width, i32 framebuffer_height,
                     const char *title, i32 surface_width, i32 surface_height);

void PlatformPresent();
void PlatformTerminate();

#endif // PTQ_COMMON_PLATFORM_H_
