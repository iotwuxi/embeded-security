/*
 *  Simple DTLS client demonstration program
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

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

#if !defined(MBEDTLS_SSL_CLI_C) || !defined(MBEDTLS_SSL_PROTO_DTLS) ||    \
    !defined(MBEDTLS_NET_C)  || !defined(MBEDTLS_TIMING_C) ||             \
    !defined(MBEDTLS_ENTROPY_C) || !defined(MBEDTLS_CTR_DRBG_C) ||        \
    !defined(MBEDTLS_X509_CRT_PARSE_C) || !defined(MBEDTLS_CERTS_C)
int main( void )
{
    mbedtls_printf( "MBEDTLS_SSL_CLI_C and/or MBEDTLS_SSL_PROTO_DTLS and/or "
            "MBEDTLS_NET_C and/or MBEDTLS_TIMING_C and/or "
            "MBEDTLS_ENTROPY_C and/or MBEDTLS_CTR_DRBG_C and/or "
            "MBEDTLS_X509_CRT_PARSE_C and/or MBEDTLS_RSA_C and/or "
            "MBEDTLS_CERTS_C and/or MBEDTLS_PEM_PARSE_C not defined.\n" );
    return( 0 );
}
#else

#include <string.h>

#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/timing.h"

#if 1
#define SERVER_PORT                     "4433"
#define SERVER_NAME                     "wsncoap.org"
#define SERVER_ADDR                     "127.0.0.1" /* forces IPv4 */
#define MESSAGE                         "Echo this"
#else
#define SERVER_PORT                     "4433"
#define SERVER_NAME                     "wsncoap.org"
#define SERVER_ADDR                     "139.196.187.107" 
#define MESSAGE                         "Hello DTLS Server"
#endif

/** DER 格式根证书，CN=CA,O=iot-wuxi,C=CN */
unsigned char ca_crt_der[] = {
  0X30, 0X82, 0X01, 0XA0, 0X30, 0X82, 0X01, 0X43, 0XA0, 0X03, 0X02, 0X01,
  0X02, 0X02, 0X01, 0X01, 0X30, 0X0C, 0X06, 0X08, 0X2A, 0X86, 0X48, 0XCE,
  0X3D, 0X04, 0X03, 0X02, 0X05, 0X00, 0X30, 0X2D, 0X31, 0X0B, 0X30, 0X09,
  0X06, 0X03, 0X55, 0X04, 0X03, 0X13, 0X02, 0X43, 0X41, 0X31, 0X11, 0X30,
  0X0F, 0X06, 0X03, 0X55, 0X04, 0X0A, 0X13, 0X08, 0X69, 0X6F, 0X74, 0X2D,
  0X77, 0X75, 0X78, 0X69, 0X31, 0X0B, 0X30, 0X09, 0X06, 0X03, 0X55, 0X04,
  0X06, 0X13, 0X02, 0X43, 0X4E, 0X30, 0X20, 0X17, 0X0D, 0X31, 0X37, 0X30,
  0X37, 0X32, 0X36, 0X30, 0X30, 0X30, 0X30, 0X30, 0X30, 0X5A, 0X18, 0X0F,
  0X32, 0X30, 0X36, 0X37, 0X30, 0X37, 0X32, 0X36, 0X30, 0X30, 0X30, 0X30,
  0X30, 0X30, 0X5A, 0X30, 0X2D, 0X31, 0X0B, 0X30, 0X09, 0X06, 0X03, 0X55,
  0X04, 0X03, 0X13, 0X02, 0X43, 0X41, 0X31, 0X11, 0X30, 0X0F, 0X06, 0X03,
  0X55, 0X04, 0X0A, 0X13, 0X08, 0X69, 0X6F, 0X74, 0X2D, 0X77, 0X75, 0X78,
  0X69, 0X31, 0X0B, 0X30, 0X09, 0X06, 0X03, 0X55, 0X04, 0X06, 0X13, 0X02,
  0X43, 0X4E, 0X30, 0X59, 0X30, 0X13, 0X06, 0X07, 0X2A, 0X86, 0X48, 0XCE,
  0X3D, 0X02, 0X01, 0X06, 0X08, 0X2A, 0X86, 0X48, 0XCE, 0X3D, 0X03, 0X01,
  0X07, 0X03, 0X42, 0X00, 0X04, 0X30, 0X7F, 0XA0, 0X42, 0XD7, 0X8E, 0X07,
  0X1B, 0X10, 0X86, 0XC5, 0X14, 0X8D, 0X7F, 0X45, 0X4F, 0X9A, 0X90, 0X6E,
  0XD3, 0X50, 0X78, 0X60, 0X53, 0XB4, 0X92, 0XCE, 0X82, 0XD2, 0X9B, 0XA0,
  0X89, 0X1D, 0X48, 0X3E, 0XCA, 0XBF, 0X0A, 0X1C, 0X65, 0X5A, 0XF7, 0X25,
  0XFB, 0XBD, 0XED, 0X33, 0X49, 0X2B, 0XCB, 0XB4, 0XCE, 0X77, 0X91, 0X96,
  0X64, 0XFA, 0XFB, 0X2B, 0X5A, 0X2E, 0XF4, 0X25, 0XC7, 0XA3, 0X50, 0X30,
  0X4E, 0X30, 0X0C, 0X06, 0X03, 0X55, 0X1D, 0X13, 0X04, 0X05, 0X30, 0X03,
  0X01, 0X01, 0XFF, 0X30, 0X1D, 0X06, 0X03, 0X55, 0X1D, 0X0E, 0X04, 0X16,
  0X04, 0X14, 0XE7, 0XFF, 0X7E, 0X48, 0X31, 0X2A, 0X8B, 0XD6, 0X58, 0XD5,
  0X81, 0X2F, 0XCD, 0X18, 0X73, 0X92, 0X4B, 0XA9, 0X3A, 0XF3, 0X30, 0X1F,
  0X06, 0X03, 0X55, 0X1D, 0X23, 0X04, 0X18, 0X30, 0X16, 0X80, 0X14, 0XE7,
  0XFF, 0X7E, 0X48, 0X31, 0X2A, 0X8B, 0XD6, 0X58, 0XD5, 0X81, 0X2F, 0XCD,
  0X18, 0X73, 0X92, 0X4B, 0XA9, 0X3A, 0XF3, 0X30, 0X0C, 0X06, 0X08, 0X2A,
  0X86, 0X48, 0XCE, 0X3D, 0X04, 0X03, 0X02, 0X05, 0X00, 0X03, 0X49, 0X00,
  0X30, 0X46, 0X02, 0X21, 0X00, 0XD4, 0XE7, 0X68, 0X18, 0X6E, 0XEA, 0XE9,
  0X55, 0XCE, 0XEC, 0XAC, 0XC8, 0XA9, 0XE6, 0X10, 0X06, 0XAE, 0X69, 0X00,
  0X2F, 0XC0, 0X9B, 0X61, 0X08, 0XAB, 0X7F, 0X71, 0X80, 0X2D, 0XE8, 0XBB,
  0X05, 0X02, 0X21, 0X00, 0XE8, 0X7F, 0X48, 0XD5, 0X08, 0X36, 0XFF, 0XD6,
  0XFE, 0XEA, 0X34, 0XE0, 0XAB, 0X26, 0X15, 0XF9, 0XD7, 0XE8, 0XA1, 0X11,
  0XAA, 0XA0, 0XF3, 0XFB, 0X60, 0X69, 0X61, 0XF2, 0X87, 0X5B, 0XC0, 0X77
};
unsigned int ca_crt_der_len = 420;

#define READ_TIMEOUT_MS 1000
#define MAX_RETRY       5

#define DEBUG_LEVEL 0

static void my_debug( void *ctx, int level,
                      const char *file, int line,
                      const char *str )
{
    ((void) level);

    mbedtls_fprintf( (FILE *) ctx, "%s:%04d: %s", file, line, str );
    fflush(  (FILE *) ctx  );
}

int main( int argc, char *argv[] )
{
    int ret, len;
    mbedtls_net_context server_fd;
    uint32_t flags;
    unsigned char buf[1024];
    const char *pers = "dtls_client";
    int retry_left = MAX_RETRY;

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_timing_delay_context timer;

    ((void) argc);
    ((void) argv);

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold( DEBUG_LEVEL );
#endif

    /*
     * 0. Initialize the RNG and the session data
     */
    mbedtls_net_init( &server_fd );
    mbedtls_ssl_init( &ssl );
    mbedtls_ssl_config_init( &conf );
    mbedtls_x509_crt_init( &cacert );
    mbedtls_ctr_drbg_init( &ctr_drbg );

    mbedtls_printf( "\n  . Seeding the random number generator..." );
    fflush( stdout );

    mbedtls_entropy_init( &entropy );
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *) pers,
                               strlen( pers ) ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\n" );

    /*
     * 0. Load certificates
     */
    mbedtls_printf( "  . Loading the CA root certificate ..." );
    fflush( stdout );

    ret = mbedtls_x509_crt_parse( &cacert, (const unsigned char *) ca_crt_der,
                          ca_crt_der_len );
    if( ret < 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509_crt_parse returned -0x%x\n\n", -ret );
        goto exit;
    }

    mbedtls_printf( " ok (%d skipped)\n", ret );

    /*
     * 1. Start the connection
     */
    mbedtls_printf( "  . Connecting to udp/%s/%s...", SERVER_NAME, SERVER_PORT );
    fflush( stdout );

    if( ( ret = mbedtls_net_connect( &server_fd, SERVER_ADDR,
                                         SERVER_PORT, MBEDTLS_NET_PROTO_UDP ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_net_connect returned %d\n\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\n" );

    /*
     * 2. Setup stuff
     */
    mbedtls_printf( "  . Setting up the DTLS structure..." );
    fflush( stdout );

    if( ( ret = mbedtls_ssl_config_defaults( &conf,
                   MBEDTLS_SSL_IS_CLIENT,
                   MBEDTLS_SSL_TRANSPORT_DATAGRAM,
                   MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret );
        goto exit;
    }

    /* OPTIONAL is usually a bad choice for security, but makes interop easier
     * in this simplified example, in which the ca chain is hardcoded.
     * Production code should set a proper ca chain and use REQUIRED. */
    mbedtls_ssl_conf_authmode( &conf, MBEDTLS_SSL_VERIFY_OPTIONAL );
    mbedtls_ssl_conf_ca_chain( &conf, &cacert, NULL );
    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );
    mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );

    if( ( ret = mbedtls_ssl_setup( &ssl, &conf ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret );
        goto exit;
    }

    if( ( ret = mbedtls_ssl_set_hostname( &ssl, SERVER_NAME ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ssl_set_hostname returned %d\n\n", ret );
        goto exit;
    }

    mbedtls_ssl_set_bio( &ssl, &server_fd,
                         mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout );

    mbedtls_ssl_set_timer_cb( &ssl, &timer, mbedtls_timing_set_delay,
                                            mbedtls_timing_get_delay );

    mbedtls_printf( " ok\n" );

    /*
     * 4. Handshake
     */
    mbedtls_printf( "  . Performing the SSL/TLS handshake..." );
    fflush( stdout );

    do ret = mbedtls_ssl_handshake( &ssl );
    while( ret == MBEDTLS_ERR_SSL_WANT_READ ||
           ret == MBEDTLS_ERR_SSL_WANT_WRITE );

    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ssl_handshake returned -0x%x\n\n", -ret );
        goto exit;
    }

    mbedtls_printf( " ok\n" );

    /*
     * 5. Verify the server certificate
     */
    mbedtls_printf( "  . Verifying peer X.509 certificate..." );

    /* In real life, we would have used MBEDTLS_SSL_VERIFY_REQUIRED so that the
     * handshake would not succeed if the peer's cert is bad.  Even if we used
     * MBEDTLS_SSL_VERIFY_OPTIONAL, we would bail out here if ret != 0 */
    if( ( flags = mbedtls_ssl_get_verify_result( &ssl ) ) != 0 )
    {
        char vrfy_buf[512];

        mbedtls_printf( " failed\n" );

        mbedtls_x509_crt_verify_info( vrfy_buf, sizeof( vrfy_buf ), "  ! ", flags );

        mbedtls_printf( "%s\n", vrfy_buf );
    }
    else
        mbedtls_printf( " ok\n" );

    /*
     * 6. Write the echo request
     */
send_request:
    mbedtls_printf( "  > Write to server:" );
    fflush( stdout );

    len = sizeof( MESSAGE ) - 1;

    do ret = mbedtls_ssl_write( &ssl, (unsigned char *) MESSAGE, len );
    while( ret == MBEDTLS_ERR_SSL_WANT_READ ||
           ret == MBEDTLS_ERR_SSL_WANT_WRITE );

    if( ret < 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ssl_write returned %d\n\n", ret );
        goto exit;
    }

    len = ret;
    mbedtls_printf( " %d bytes written\n\n%s\n\n", len, MESSAGE );

    /*
     * 7. Read the echo response
     */
    mbedtls_printf( "  < Read from server:" );
    fflush( stdout );

    len = sizeof( buf ) - 1;
    memset( buf, 0, sizeof( buf ) );

    do ret = mbedtls_ssl_read( &ssl, buf, len );
    while( ret == MBEDTLS_ERR_SSL_WANT_READ ||
           ret == MBEDTLS_ERR_SSL_WANT_WRITE );

    if( ret <= 0 )
    {
        switch( ret )
        {
            case MBEDTLS_ERR_SSL_TIMEOUT:
                mbedtls_printf( " timeout\n\n" );
                if( retry_left-- > 0 )
                    goto send_request;
                goto exit;

            case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
                mbedtls_printf( " connection was closed gracefully\n" );
                ret = 0;
                goto close_notify;

            default:
                mbedtls_printf( " mbedtls_ssl_read returned -0x%x\n\n", -ret );
                goto exit;
        }
    }

    len = ret;
    mbedtls_printf( " %d bytes read\n\n%s\n\n", len, buf );

    /*
     * 8. Done, cleanly close the connection
     */
close_notify:
    mbedtls_printf( "  . Closing the connection..." );

/** 暂时注释掉客户端 close_notify 操作 
    由于服务器与客户端 close_notify 操作顺序的不确定性，
    使服务器侧有概率出现 -0x7900 的握手错误信息.
*/
#if 0 
    /* No error checking, the connection might be closed already */
    do ret = mbedtls_ssl_close_notify( &ssl );
    while( ret == MBEDTLS_ERR_SSL_WANT_WRITE );
#endif

    ret = 0;

    mbedtls_printf( " done\n" );

    /*
     * 9. Final clean-ups and exit
     */
exit:

#ifdef MBEDTLS_ERROR_C
    if( ret != 0 )
    {
        char error_buf[100];
        mbedtls_strerror( ret, error_buf, 100 );
        mbedtls_printf( "Last error was: %d - %s\n\n", ret, error_buf );
    }
#endif

    mbedtls_net_free( &server_fd );

    mbedtls_x509_crt_free( &cacert );
    mbedtls_ssl_free( &ssl );
    mbedtls_ssl_config_free( &conf );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );

#if defined(_WIN32)
    mbedtls_printf( "  + Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    /* Shell can not handle large exit numbers -> 1 for errors */
    if( ret < 0 )
        ret = 1;

    return( ret );
}
#endif /* MBEDTLS_SSL_CLI_C && MBEDTLS_SSL_PROTO_DTLS && MBEDTLS_NET_C &&
          MBEDTLD_TIMING_C && MBEDTLS_ENTROPY_C && MBEDTLS_CTR_DRBG_C &&
          MBEDTLS_X509_CRT_PARSE_C && MBEDTLS_RSA_C && MBEDTLS_CERTS_C &&
          MBEDTLS_PEM_PARSE_C */
