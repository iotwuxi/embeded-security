#ifndef MEMORY_ALT_H
#define MEMORY_ALT_H

#include "FreeRTOS.h"
#include "task.h"

void *platform_calloc(size_t n, size_t size);
void platform_free(void* ptr);

#endif

