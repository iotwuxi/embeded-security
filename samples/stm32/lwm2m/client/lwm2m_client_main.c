/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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
 *    Benjamin Cabé - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Julien Vermillard - Please refer to git log
 *    Axel Lorente - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    Christian Renz - Please refer to git log
 *    Ricky Liu - Please refer to git log
 *
 *******************************************************************************/

/*
 Copyright (c) 2013, 2014 Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

 David Navarro <david.navarro@intel.com>
 Bosch Software Innovations GmbH - Please refer to git log

*/

#include <stdlib.h>
#include <ctype.h>
#include <ascii.h>

#include "lwm2m_client_main.h"


// dtls
#define LWM2M_CLIENT_WITH_DTLS  1

#if LWM2M_CLIENT_WITH_DTLS
#define PSKID           "PSK_identity"
#define PASSWORLD       "11111111"
#define SERVER_NAME     "coaps://wsncoap.org"
#define SERVER_PORT     "6684"
#endif

/*
 * Definition for handling pthread
 */
#define LWM2M_CLIENT_PRIORITY     100
#define LWM2M_CLIENT_STACK_SIZE   16384
#define LWM2M_CLIENT_SCHED_POLICY SCHED_RR

#define OBJ_COUNT 9
# define BACKUP_OBJECT_COUNT 2

int g_reboot = 0;
static int g_quit = 0;

static char coap_uri_prefix [COAP_PROTOCOL_MAX][COAP_MAX_URI_PREFIX_SIZE] = {
    "coap://",
    "coaps://",
    "coap+tcp://",
    "coaps+tcp://"
};

int g_lifetime = 300;

/* PSK for example. Please do not use it in commercial version */
char *g_pskId = "testpsk";
uint16_t g_pskLen = 8;
char *g_pskBuffer = "12345678";

char *g_step[6] = {"STATE_INITIAL", "STATE_BOOTSTRAP_REQUIRED", "STATE_BOOTSTRAPPING", \
                   "STATE_REGISTER_REQUIRED", "STATE_REGISTERING", "STATE_READY"
                  };
const char *server = DEFAULT_SERVER_IPV4;
const char *serverPort = LWM2M_STANDARD_PORT_STR;
coap_protocol_t g_proto = COAP_UDP;
client_data_t data;
lwm2m_context_t *lwm2mH;
lwm2m_object_t *objArray[OBJ_COUNT];
lwm2m_object_t *backupObjectArray[BACKUP_OBJECT_COUNT];

void handle_value_changed(lwm2m_context_t *lwm2mP, lwm2m_uri_t *uri, const char *value, size_t valueLength)
{
    lwm2m_object_t *object = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mP->objectList, uri->objectId);

    if (NULL != object) {
        if (object->writeFunc != NULL) {
            lwm2m_data_t *dataP;
            int result;

            dataP = lwm2m_data_new(1);
            if (dataP == NULL) {
                fprintf(stderr, "Internal allocation failure !\n");
                return;
            }
            dataP->id = uri->resourceId;
            lwm2m_data_encode_nstring(value, valueLength, dataP);

            result = object->writeFunc(uri->instanceId, 1, dataP, object);
            if (COAP_405_METHOD_NOT_ALLOWED == result) {
                switch (uri->objectId) {
                case LWM2M_DEVICE_OBJECT_ID:
                    result = device_change(dataP, object);
                    break;
                default:
                    break;
                }
            }

            if (COAP_204_CHANGED != result) {
                fprintf(stderr, "Failed to change value!\n");
            } else {
                fprintf(stderr, "value changed!\n");
                lwm2m_resource_value_changed(lwm2mP, uri);
            }
            lwm2m_data_free(1, dataP);
            return;
        } else {
            fprintf(stderr, "write not supported for specified resource!\n");
        }
        return;
    } else {
        fprintf(stderr, "Object not found !\n");
    }
}

static char *coap_get_port_from_proto(coap_protocol_t proto)
{
    char *port = NULL;

    switch (proto) {
    case COAP_TCP:
    case COAP_UDP:
        port = LWM2M_STANDARD_PORT_STR;
        break;
    case COAP_TCP_TLS:
    case COAP_UDP_DTLS:
        port = LWM2M_DTLS_PORT_STR;
        break;
    default:
        break;
    }

    return port;
}

static coap_protocol_t coap_get_protocol_from_uri(const char *uri)
{
    coap_protocol_t type;
    for (type = COAP_UDP; type < COAP_PROTOCOL_MAX; type++) {
        if (!strncmp(uri, coap_uri_prefix[type], strlen(coap_uri_prefix[type]))) {
            return type;
        }
    }
    return type;
}

void *lwm2m_connect_server(uint16_t secObjInstID, void *userData)
{
    client_data_t *dataP;
    char *uri;
    char *host;
    char *port;
    connection_t *newConnP = NULL;
    coap_protocol_t proto = COAP_UDP;

    dataP = (client_data_t *)userData;

    uri = get_server_uri(dataP->securityObjP, secObjInstID);

    if (uri == NULL) {
        return NULL;
    }

    // parse uri in the form "coaps://[host]:[port]"

    proto = coap_get_protocol_from_uri(uri);
    if (proto >= COAP_PROTOCOL_MAX) {
        fprintf(stderr, "Not supported protocol : %d\n", proto);
        goto exit;
    } else {
        /* move pointer to address field */
        host = uri + strlen(coap_uri_prefix[proto]);
    }

    port = strrchr(host, ':');
    if (port == NULL) {
        goto exit;
    }
    // remove brackets
    if (host[0] == '[') {
        host++;
        if (*(port - 1) == ']') {
            *(port - 1) = 0;
        } else {
            goto exit;
        }
    }
    // split strings
    *port = 0;
    port++;

    fprintf(stdout, "Opening connection to server at %s:%s\r\n", host, port);
    fprintf(stdout, "Connection protocol type : %d\r\n", proto);
    newConnP = connection_create(proto, dataP->connP, dataP->sock, host, port, dataP->addressFamily);
    if (newConnP == NULL) {
        fprintf(stderr, "Connection creation failed.\r\n");
        return NULL;
    } else {
        memcpy(&dataP->server_addr, &newConnP->addr, newConnP->addrLen);
        dataP->server_addrLen = newConnP->addrLen;
        dataP->connP = newConnP;

#ifdef WITH_MBEDTLS
        if (proto == COAP_TCP_TLS || proto == COAP_UDP_DTLS) {
            newConnP->session = TLSSession(newConnP->sock, dataP->tls_context, dataP->tls_opt);
            if (newConnP->session == NULL) {
                fprintf(stderr, "Failed to create secure session. \r\n");
                connection_free(newConnP);
                newConnP = NULL;
                goto exit;
            }
            dataP->sock = newConnP->session->net.fd;
            fprintf(stdout, "successfully create secure session. \r\n");
        }
#endif
    }

exit:
    lwm2m_free(uri);
    return (void *)newConnP;
}

void lwm2m_close_connection(void *sessionH, void *userData)
{
    client_data_t *app_data;
    connection_t *targetP;
    app_data = (client_data_t *)userData;
    targetP = (connection_t *)sessionH;

    if (targetP == app_data->connP) {
        app_data->connP = targetP->next;
        lwm2m_free(targetP);
    } else {
        connection_t *parentP;

        parentP = app_data->connP;
        while (parentP != NULL && parentP->next != targetP) {
            parentP = parentP->next;
        }
        if (parentP != NULL) {
            parentP->next = targetP->next;
            lwm2m_free(targetP);
        }
    }

#ifdef WITH_MBEDTLS
    if (targetP->session) {
        TLSSession_free(targetP->session);
    }
#endif
}


#ifdef WITH_MBEDTLS

#define HEX2NUM(c)              \
if (c >= '0' && c <= '9')       \
    c -= '0';                   \
else if (c >= 'a' && c <= 'f')  \
    c -= 'a' - 10;              \
else if (c >= 'A' && c <= 'F')  \
    c -= 'A' - 10;              \
else                            \
    return(-1);

int lwm2m_unhexify(unsigned char *output, const char *input, size_t *olen)
{
    unsigned char c;
    size_t j;

    *olen = strlen(input);
    if (*olen % 2 != 0 || *olen / 2 > MBEDTLS_PSK_MAX_LEN) {
        return (-1);
    }
    *olen /= 2;

    for (j = 0; j < *olen * 2; j += 2) {
        c = input[j];
        HEX2NUM(c);
        output[j / 2] = c << 4;

        c = input[j + 1];
        HEX2NUM(c);
        output[j / 2] |= c;
    }

    return (0);
}
#endif

int lwm2m_init_object(void)
{
    char serverUri[50];
    int serverId = 123;

    /* Fill an array with each object. */
    sprintf(serverUri, "%s%s:%s", coap_uri_prefix[g_proto], server, serverPort);
    objArray[0] = get_security_object(serverId, serverUri, g_pskId, g_pskBuffer, g_pskLen, false);
    if (NULL == objArray[0]) {
        fprintf(stderr, "Failed to create security object\r\n");
        return -1;
    }
    data.securityObjP = objArray[0];
    /*
     * Bind Accordingly Protocol (e.g., TCP, UDP)
     * get_server_object(serverId, "T", lifetime, false);
     */
    switch (g_proto) {
    case COAP_TCP:
    case COAP_TCP_TLS:
        objArray[1] = get_server_object(serverId, "T", g_lifetime, false);
        break;
    case COAP_UDP:
    case COAP_UDP_DTLS:
        objArray[1] = get_server_object(serverId, "U", g_lifetime, false);
        break;
    default:
        fprintf(stderr, "Cannot get_server_object with protocol %d\n", g_proto);
        break;
    }

    if (NULL == objArray[1]) {
        fprintf(stderr, "Failed to create server object\r\n");
        return -1;
    }

    objArray[2] = get_object_device();
    if (NULL == objArray[2]) {
        fprintf(stderr, "Failed to create Device object\r\n");
        return -1;
    }

    objArray[3] = get_object_firmware();
    if (NULL == objArray[3]) {
        fprintf(stderr, "Failed to create Firmware object\r\n");
        return -1;
    }

    objArray[4] = get_object_location();
    if (NULL == objArray[4]) {
        fprintf(stderr, "Failed to create location object\r\n");
        return -1;
    }

    objArray[5] = get_test_object();
    if (NULL == objArray[5]) {
        fprintf(stderr, "Failed to create test object\r\n");
        return -1;
    }

    objArray[6] = get_object_conn_m();
    if (NULL == objArray[6]) {
        fprintf(stderr, "Failed to create connectivity monitoring object\r\n");
        return -1;
    }

    objArray[7] = get_object_conn_s();
    if (NULL == objArray[7]) {
        fprintf(stderr, "Failed to create connectivity statistics object\r\n");
        return -1;
    }

    int instId = 0;
    objArray[8] = acc_ctrl_create_object();
    if (NULL == objArray[8]) {
        fprintf(stderr, "Failed to create Access Control object\r\n");
        return -1;
    } else if (acc_ctrl_obj_add_inst(objArray[8], instId, 3, 0, serverId) == false) {
        fprintf(stderr, "Failed to create Access Control object instance\r\n");
        return -1;
    } else if (acc_ctrl_oi_add_ac_val(objArray[8], instId, 0, 0x0f) == false) {
        fprintf(stderr, "Failed to create Access Control ACL default resource\r\n");
        return -1;
    } else if (acc_ctrl_oi_add_ac_val(objArray[8], instId, 999, 0x01) == false) {
        fprintf(stderr, "Failed to create Access Control ACL resource for serverId: 999\r\n");
        return -1;
    }
    return 0;
}

void lwm2m_client_thread(void* args)
{
    int i, result;

    char *name = "testlwm2mclient";

#ifdef WITH_MBEDTLS
    unsigned char psk[MBEDTLS_PSK_MAX_LEN];

    /* set default tls option */

    /*
     * if you want to change auth_mode, please change 3rd parameter of tls_opt structure
     * - auth_mode can be configured (2: mandatory 1: optional, 0: not verify)
     */
    tls_opt option = {MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_DATAGRAM, 
                        0, 1, NULL, {mbedtls_ssl_get_ciphersuite_id(LWM2M_CIPHERSUIT), 0}};
#endif

    struct timeval tv = {60, 0};

    lwm2mH = NULL;
    g_quit = 0;
    server = DEFAULT_SERVER_IPV4;
    serverPort = LWM2M_STANDARD_PORT_STR;

#if LWM2M_CLIENT_WITH_DTLS
    g_pskId = PSKID;
    g_pskBuffer = PASSWORLD;
    server = SERVER_NAME;
    serverPort = SERVER_PORT;   
#endif

    memset(&data, 0, sizeof(client_data_t));
    data.addressFamily = AF_INET;

    /* Parse server URI to distinguish protocol and server address */
    g_proto = coap_get_protocol_from_uri(server);
    if (g_proto >= COAP_PROTOCOL_MAX) {
        printf("Not supported protocol : %d\n", g_proto);
        return;
    }

    /* Move pointer to address field */
    server += strlen(coap_uri_prefix[g_proto]);
    serverPort = coap_get_port_from_proto(g_proto);

    if (lwm2m_init_object() < 0) {
    }

#if 1
    data.sock = create_socket(g_proto, serverPort, data.addressFamily);

    if (data.sock < 0) {
        fprintf(stderr, "Failed to open socket: %d %s\r\n", errno, strerror(errno));
        return;
    }
#endif

#ifdef WITH_MBEDTLS
    if (g_proto == COAP_TCP_TLS || g_proto == COAP_UDP_DTLS) {

        /* Set Transport layer (TCP or UDP) */
        switch (g_proto) {
        case COAP_TCP_TLS:
            option.transport = MBEDTLS_SSL_TRANSPORT_STREAM;
            break;
        case COAP_UDP_DTLS:
            option.transport = MBEDTLS_SSL_TRANSPORT_DATAGRAM;
            break;
        default:
            break;
        }
        data.tls_opt = &option;

        /* Set credential information */
        tls_cred cred;
        memset(&cred, 0, sizeof(tls_cred));

        if (g_pskBuffer) {
            if (lwm2m_unhexify(psk, g_pskBuffer, &cred.psk_len) == 0) {
                if (g_pskId) {
                    cred.psk_identity = g_pskId;
                    cred.psk = psk;
                }
            }
            if (cred.psk_identity == NULL && cred.psk == NULL) {
                printf("failed to set psk info\n");
                goto exit;
            }
        } else {
            printf("Please set psk and psk_id\n");
            goto exit;
        }

        data.tls_context = TLSCtx(&cred);
        if (data.tls_context == NULL) {
            printf("TLS context initialize failed\n");
            goto exit;
        }
    }
#endif

    if ((lwm2mH = lwm2m_init(&data)) == NULL) {
        printf("lwm2m_init2() failed\n");
        goto exit;
    }
    lwm2mH->protocol = g_proto;

    if ((result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray)) != 0) {
        printf("lwm2m_configure() failed: 0x%X\n", result);
        goto exit;
    }

    printf("LWM2M Client \"%s\" started \n>  ", name);

    /* We now enter in a while loop that will handle the communications from the server */
    while (0 == g_quit) {
        tv.tv_sec = 60;
        tv.tv_usec = 0;

        if ((result = lwm2m_step(lwm2mH, (time_t *)&(tv.tv_sec))) != 0) {
            printf("lwm2m_step() failed: 0x%X\n", result);
            goto exit;
        } else {
            printf(" -> State: %s\n", g_step[lwm2mH->state]);
        }

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(data.sock, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);
        if (result < 0) {
            if (errno != EINTR) {
                fprintf(stderr, "Error in select(): %d %s\r\n", errno, strerror(errno));
            }
        } else if (result > 0) {
            uint8_t buffer[MAX_PACKET_SIZE];
            int numBytes;

            if (FD_ISSET(data.sock, &readfds)) {
                client_data_t *user_data = lwm2mH->userData;

                numBytes = connection_read(g_proto, user_data->connP, data.sock, buffer, MAX_PACKET_SIZE, NULL, 0);

                if (numBytes > 0) {
                    output_buffer(stderr, buffer, numBytes, 0);
                    lwm2m_handle_packet(lwm2mH, buffer, numBytes, user_data->connP);
                    conn_s_updateRxStatistic(objArray[7], numBytes, false);
                } else {
                    printf("received bytes ignored!\n");
                }
                /* If the event happened on the SDTIN */
            } 
        }
    }

exit:
    if (g_quit && lwm2mH) {
        lwm2m_close(lwm2mH);
    }

#ifdef WITH_MBEDTLS
    if (data.tls_context) {
        TLSCtx_free(data.tls_context);
    }
#endif

    if (data.sock >= 0) {
        shutdown(data.sock, 3);
        if ((i = close(data.sock)) != 0) {
            printf("Fail to close %d\n", errno);
        }
    }

    if (data.connP) {
        // 需要修改 - xianrenqiu
        // connection_free(data.connP);
    }

    clean_security_object(objArray[0]);
    clean_server_object(objArray[1]);
    free_object_device(objArray[2]);
    free_object_firmware(objArray[3]);
    free_object_location(objArray[4]);
    free_test_object(objArray[5]);
    free_object_conn_m(objArray[6]);
    free_object_conn_s(objArray[7]);
    acl_ctrl_free_object(objArray[8]);
    lwm2m_free(objArray[0]);
    lwm2m_free(objArray[1]);

    return;
}


void lwm2m_client_init()
{
    sys_thread_new("LWM2M Client", lwm2m_client_thread, NULL, DEFAULT_THREAD_STACKSIZE, LWM2M_CLIENT_THREAD_PRIO);
}


void sample_entry(void)
{
    /* 注册任务接口，dhcp成功后开始执行 */
    app_net_register_thread(lwm2m_client_init);

    app_ethernet_init();
}
