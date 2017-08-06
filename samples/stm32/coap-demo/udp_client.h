#ifndef __UDP_CLIENT_H
#define __UDP_CLIENT_H

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"

#include "cmsis_os.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

#include "ethernetif.h"
#include "app_ethernet.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>


#define UDP_CLIENT_THREAD_PRIO      (osPriorityNormal)

/** netcat -l -u 139.196.187.107 -p 5000 & */

#define SERV_PORT                   5000
#define SERVER_IP                   "139.196.187.107"

#define MAXSIZE                     256


#endif