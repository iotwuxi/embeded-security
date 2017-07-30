
#include "cipher_md_list.h"

void cipher_md_list_thread(void const * argument)
{
    const int *list;
    int index = 1;
    const mbedtls_md_info_t *md_info;    
    const mbedtls_cipher_info_t *cipher_info;

    printf( "Available ciphers:\n" );
    list = mbedtls_cipher_list();
    while( *list )
    {
        cipher_info = mbedtls_cipher_info_from_type((mbedtls_cipher_type_t)*list);
        printf( "\t[%02d]%s\n", index++, cipher_info->name );
        list++; 
    }

    printf( "\nAvailable message digests:\n" );
    index = 1;
    list = mbedtls_md_list();
    while( *list )
    {
        md_info = mbedtls_md_info_from_type((mbedtls_md_type_t)*list);
        printf( "\t[%02d]%s\n", index++, mbedtls_md_get_name(md_info));
        list++;
    }
    
    printf( "\nAvailable Ciphersuite:\n" );
    index = 1;
    list = mbedtls_ssl_list_ciphersuites();
    while( *list )
    {
        mbedtls_printf("\t[%02d]%s\n", index++, mbedtls_ssl_get_ciphersuite_name(*list));
        list++; 
    }
        
    for(;;) 
    {
        /* 删除启动任务 */ 
        osThreadTerminate(NULL);
    }
        
}

void app_init(void)
{
    osThreadDef(cipher_md_list, cipher_md_list_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
    osThreadCreate(osThread(cipher_md_list), NULL);
}
