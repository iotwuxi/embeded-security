/******************************************************************************
 *
 * Copyright(c) 2007 - 2015 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/
#include <platform_opts.h>
#include "main.h"

/*
  	All of the examples are disabled by default for code size consideration
   	The configuration is enabled in platform_opts.h
*/
void example_entry(void)
{
#if CONFIG_EXAMPLE_DTLS_PSK_CLIENT
    example_dtls_psk_client();
#endif
        
/** add your own example code */
#if CONFIG_EXAMPLE_coaps_CLIENT
    example_coaps_client();
#endif
}
