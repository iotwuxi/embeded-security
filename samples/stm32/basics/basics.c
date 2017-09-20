
#include "basics.h"

void sample_entry(void)
{
	mbedtls_platform_set_calloc_free(platform_calloc, platform_free);
    
    sample_cipher_md();
    sample_x509();
    sample_aes();
    
    size_t free = xPortGetFreeHeapSize();
    printf("\nFree: %zd\n", free);
    sample_ctr_random();
    
    free = xPortGetFreeHeapSize();
    printf("\nFree: %zd\n", free);
    sample_hmac_random();
}
