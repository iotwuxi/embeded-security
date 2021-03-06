#ifndef __DTLS_CLIENT_H
#define __DTLS_CLIENT_H

/* include */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf     printf
#define mbedtls_fprintf    fprintf
#endif

#include <string.h>

#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/timing.h"

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"

#include "cmsis_os.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

#include "ethernetif.h"
#include "app_ethernet.h"


/* define */
#define DEBUG_LEVEL 					(0)
#define SERVER_PORT 					"4433"
#define SERVER_NAME 					"wsncoap.org"
#define SERVER_ADDR 					"139.196.187.107" 
#define MESSAGE     					"Hello DTLS Server"
#define MAX_RETRY       				(5)
#define READ_TIMEOUT_MS 				(1000)
#define DTLS_CLIENT_THREAD_PRIO      	(osPriorityNormal)

#endif