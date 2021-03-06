/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *
 *******************************************************************************/

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "liblwm2m.h"
#ifdef WITH_MBEDTLS
#include "easy_tls.h"
#endif

#include <stdio.h>

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#define LWM2M_STANDARD_PORT_STR "6683"
#define LWM2M_STANDARD_PORT      6683
#define LWM2M_DTLS_PORT_STR     "6684"
#define LWM2M_DTLS_PORT          6684
#define LWM2M_BSSERVER_PORT_STR "6685"
#define LWM2M_BSSERVER_PORT      6685

typedef struct _connection_t {
	struct _connection_t	*next;
	int						sock;
#ifdef __ICCARM__  
    struct sockaddr_in		addr;
#else
	struct sockaddr_in6		addr;
#endif
    size_t					addrLen;
#ifdef WITH_MBEDTLS
	tls_session *session;
#endif
} connection_t;

int create_socket(coap_protocol_t protocol, const char *portStr, int ai_family);
connection_t *create_session(int sockfd, struct sockaddr_storage *caddr, socklen_t *caddrLen, coap_protocol_t proto, void *tls_context, void *tls_option);
connection_t *connection_find(connection_t *connList, struct sockaddr_storage *addr, size_t addrLen);
connection_t *connection_new_incoming(connection_t *connList, int sock, struct sockaddr *addr, size_t addrLen);
connection_t *connection_create(coap_protocol_t protocol, connection_t *connList, int sock, char *host, char *port, int addressFamily);

void connection_free(connection_t *connList);

int connection_send(connection_t *connP, uint8_t *buffer, size_t length, coap_protocol_t proto);
int connection_read(coap_protocol_t protocol, connection_t *connP, int sock, uint8_t *buffer, size_t len, struct sockaddr_storage *from, socklen_t *fromLen);
#endif
