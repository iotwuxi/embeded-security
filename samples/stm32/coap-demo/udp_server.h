#ifndef __UDP_SERVER_H
#define __UDP_SERVER_H

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "cmsis_os.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

#include "ethernetif.h"
#include "app_ethernet.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define BUF_SIZE	                128
#define PORT                        5000
#define UDP_SERVER_THREAD_PRIO      (osPriorityAboveNormal)


#endif
