/******************************************************************************
 *
 * Copyright(c) 2007 - 2015 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/
#include <platform_opts.h>
#include "main.h"

#if CONFIG_EXAMPLE_HTTPD
#include <httpd/example_httpd.h>
#endif

#if CONFIG_EXAMPLE_MQTT
#include <mqtt/example_mqtt.h>
#endif

#if CONFIG_EXAMPLE_HIGH_LOAD_MEMORY_USE
#include <high_load_memory_use/example_high_load_memory_use.h>
#endif

/*
  	All of the examples are disabled by default for code size consideration
   	The configuration is enabled in platform_opts.h
*/
void example_entry(void)
{
#if CONFIG_EXAMPLE_DTLS_PSK_CLIENT
        example_dtls_psk_client();
#endif

#if CONFIG_EXAMPLE_HTTPD
	example_httpd();
#endif

#if CONFIG_EXAMPLE_MQTT
	example_mqtt();
#endif

#if CONFIG_EXAMPLE_HIGH_LOAD_MEMORY_USE
        example_high_load_memory_use();
#endif

}
