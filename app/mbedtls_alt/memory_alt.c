
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

/**
 * @brief FreeRTOS平台calloc和free
 */
void *platform_calloc(size_t n, size_t size)
{
    void *ptr = NULL; 
    ptr = pvPortMalloc(n * size);
    if (ptr != NULL) {
        memset(ptr, 0x00, n * size);
    }
        
    return ptr;
}

void platform_free(void* ptr)
{
    vPortFree(ptr);
}
