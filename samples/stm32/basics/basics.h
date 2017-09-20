#ifndef __BASICS_CLIENT_H
#define __BASICS_CLIENT_H

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

#include "mbedtls/cipher.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
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
#include "memory_alt.h"

void sample_hmac_random(void);
void sample_ctr_random(void);
void sample_cipher_md(void);
void sample_x509(void);
void sample_aes(void);

#endif