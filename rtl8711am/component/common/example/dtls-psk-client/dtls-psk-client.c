#include "FreeRTOS.h"
#include "task.h"
#include <platform_stdlib.h>
#include <lwip_netconf.h>
#include "wifi_constants.h"

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/certs.h"
#include "mbedtls/timing.h"

#define DFL_PSK                     "password"
#define DFL_PSK_IDENTITY            "identity"
#define SERVER_PORT                 "4433"
#define SERVER_NAME                 "localhost"
#define SERVER_ADDR                 "139.196.187.107" /* 阿里云 */
#define MESSAGE                     "This is RTL8711AM DTLS Client"
#define READ_TIMEOUT_MS             1000
#define MAX_RETRY                   5
#define PRIORITY_OFFSET		        1

void dtls_psk_client_thread(void);
static void dtls_psk_client_handler(void);

/*
 * @brief  dtls psk client case.
 * @note  Process Flow:
 *      - Enable Wi-Fi with STA mode
 *      - Connect to AP by WPA2-AES
 *      - Start DTLS sessions
 */
static void dtls_psk_client_usage(void)
{
    printf("\n\ndtls psk client case...\n");

    /*
     * 1. Enable Wi-Fi with STA mode
     */
    printf("\n\rEnable Wi-Fi\n");
    if(wifi_on(RTW_MODE_STA) < 0)
    {
        printf("\n\rERROR: wifi_on failed\n");
        return;
    }
    
    /*
     * 2. Connect to AP by WPA2-AES
     */
    char *ssid = "bowen";
    char *password = "xianrenqiu";
    if(wifi_connect(ssid, RTW_SECURITY_WPA2_AES_PSK, password, strlen(ssid), strlen(password), -1, NULL) == RTW_SUCCESS)
    {
        LwIP_DHCP(0, DHCP_START);
    }

    /*
     * 3. Start DTLS sessions 
     */
    dtls_psk_client_thread();
}


/*
 * @brief  SSL/TLS connection.
 * @note  Please refer to component\common\utilities\ssl_client.c.
 */
void dtls_psk_client_thread(void)
{
    if(xTaskCreate(dtls_psk_client_handler, "dtls_psk_client_handler", 1150, NULL, tskIDLE_PRIORITY + 3 + PRIORITY_OFFSET, NULL) != pdPASS)
        printf("\n\r%s xTaskCreate failed", __FUNCTION__);
}

static void* my_calloc(size_t nelements, size_t elementSize)
{
    size_t size;
    void *ptr = NULL;

    size = nelements * elementSize;
    ptr = pvPortMalloc(size);

    if(ptr)
        memset(ptr, 0, size);

    return ptr;
}

#define my_free     vPortFree

static int my_random(void *p_rng, unsigned char *output, size_t output_len)
{
    rtw_get_random_bytes(output, output_len);
    return 0;
}

static void dtls_psk_client_handler(void)
{
#if !defined(MBEDTLS_BIGNUM_C) || !defined(MBEDTLS_CERTS_C) || \
    !defined(MBEDTLS_SSL_TLS_C) || !defined(MBEDTLS_KEY_EXCHANGE_PSK_ENABLED) \
    !defined(MBEDTLS_RSA_C) || !defined(MBEDTLS_NET_C) || || !define(MBEDTLS_TIMING_C)\
    !defined(MBEDTLS_PEM_PARSE_C) || !defined(MBEDTLS_X509_CRT_PARSE_C) || \
    !defined(MBEDTLS_SSL_PROTO_DTLS) || !define(MBEDTLS_TIMING_ALT)

    printf("MBEDTLS_BIGNUM_C and/or MBEDTLS_CERTS_C and/or "
        "MBEDTLS_SSL_TLS_C and/or MBEDTLS_SSL_SRV_C and/or "
        "MBEDTLS_RSA_C and/or MBEDTLS_NET_C and/or "
        "MBEDTLS_PEM_PARSE_C and/or MBEDTLS_X509_CRT_PARSE_C and/or "
        "MBEDTLS_SSL_PROTO_DTLS and/or MBEDTLS_TIMING_C and/or "
        "MBEDTLS_TIMING_ALT not defined.\n");

#else    
    int ret, len;
    mbedtls_net_context server_fd;
    uint32_t flags;
    unsigned char buf[1024];
    int retry_left = MAX_RETRY;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_timing_delay_context timer;
    int dtls_ciphersuites[3];

    mbedtls_platform_set_calloc_free(my_calloc, vPortFree);

    /*
     * 0. Loading the psk and psk_identity
     */
    mbedtls_net_init( &server_fd );
    mbedtls_ssl_init( &ssl );
    mbedtls_ssl_config_init( &conf );
    mbedtls_x509_crt_init( &cacert );

    printf( "\n  . Loading the psk and psk_identity" );

    ret = mbedtls_ssl_conf_psk( &conf, (const unsigned char *)DFL_PSK, strlen(DFL_PSK),
                                       (const unsigned char *)DFL_PSK_IDENTITY, strlen(DFL_PSK_IDENTITY));
    if( ret != 0 )
    {
        printf( " failed\n  !  mbedtls_ssl_conf_psk %d\n\n", ret );
        goto exit;
    }

    printf( " ok\n" );  

    /*
     * 1. Start the connection
     */
    mbedtls_printf( "  . Connecting to udp/%s/%s...", SERVER_NAME, SERVER_PORT );

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

    // mbedtls_ssl_conf_ca_chain(&conf, cacert.next, NULL);
    // mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE);
    mbedtls_ssl_conf_rng( &conf, my_random, NULL );

    dtls_ciphersuites[0] = MBEDTLS_TLS_PSK_WITH_AES_128_CCM;
    dtls_ciphersuites[1] = MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8;
    dtls_ciphersuites[2] = 0;
    mbedtls_ssl_conf_ciphersuites( &conf, dtls_ciphersuites);

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

    mbedtls_ssl_set_bio( &ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout );
    mbedtls_ssl_set_timer_cb( &ssl, &timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay );

    mbedtls_printf( " ok\n" );

    /*
     * 4. Handshake
     */
    mbedtls_printf( "  . Performing the SSL/TLS handshake..." );

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

    /* No error checking, the connection might be closed already */
    do ret = mbedtls_ssl_close_notify( &ssl );
    while( ret == MBEDTLS_ERR_SSL_WANT_WRITE );
    ret = 0;

    mbedtls_printf( " done\n" );

    /*
     * 9. Final clean-ups and exit
     */
exit:
    mbedtls_net_free( &server_fd );

    mbedtls_x509_crt_free( &cacert );
    mbedtls_ssl_free( &ssl );
    mbedtls_ssl_config_free( &conf );

    /* Shell can not handle large exit numbers -> 1 for errors */
    if( ret < 0 )
        ret = 1;

#endif
    vTaskDelete(NULL);
}


static void example_dtls_psk_client_thread(void)
{
    vTaskDelay(4000);
    
    dtls_psk_client_usage();

    vTaskDelete(NULL);
}

void example_dtls_psk_client(void)
{
    if(xTaskCreate(example_dtls_psk_client_thread, ((const char*)"example_dtls_psk_client_thread"), 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        printf("\n\r%s xTaskCreate failed\n", __FUNCTION__);
}
