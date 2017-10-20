
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief FreeRTOS平台calloc和free
 */
void *platform_calloc(size_t n, size_t size)
{
    void *ptr = NULL; 
    ptr = pvPortMalloc(n * size);
    return ptr;
}

void platform_free(void* ptr)
{
    vPortFree(ptr);
}
