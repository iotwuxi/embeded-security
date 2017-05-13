#include <platform_opts.h>
#include "FreeRTOS.h"
#include "task.h"
#include <platform/platform_stdlib.h>
#include <lwip_netconf.h>
#include <lwip/sockets.h>
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "lwip_netconf.h"
#include <string.h>

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/timing.h"

#define DFL_PSK                 "password"
#define DFL_PSK_IDENTITY        "identity"

#define SERVER_PORT "4433"
#define SERVER_NAME "localhost"
#define SERVER_ADDR "139.196.187.107" /* forces IPv4 */
#define MESSAGE     "Hello DTLS Server"

#define READ_TIMEOUT_MS 1000
#define MAX_RETRY       5

#define PRIORITY_OFFSET		1

void dtls_psk_client_thread(void);
static void dtls_psk_client_handler(void);

#define DEBUG_LEVEL 0

static void my_debug( void *ctx, int level,
                      const char *file, int line,
                      const char *str )
{
    ((void) level);

    mbedtls_fprintf( /*(FILE *)*/ ctx, "%s:%04d: %s", file, line, str );
//    fflush(  (FILE *) ctx  );
}

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
   
    /**********************************************************************************
    *   1. Enable Wi-Fi with STA mode       
    **********************************************************************************/
    printf("\n\rEnable Wi-Fi\n");
    if(wifi_on(RTW_MODE_STA) < 0)
    {
        printf("\n\rERROR: wifi_on failed\n");
        return;
    }
    
    
    /**********************************************************************************
    *   2. Connect to AP by WPA2-AES        
    **********************************************************************************/ 
    // Connect to AP with PSK-WPA2-AES.
    char *ssid = "bowen";
    char *password = "xianrenqiu";
    if(wifi_connect(ssid, RTW_SECURITY_WPA2_AES_PSK, password, strlen(ssid), strlen(password), -1, NULL) == RTW_SUCCESS)
    {
        LwIP_DHCP(0, DHCP_START);
    }
    
    /**********************************************************************************
    *   3. Start DTLS sessions 
    **********************************************************************************/ 
    // Start DTLS connection
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

static char *hl_itoa(int value){
    char *val_str;
    int tmp = value, len = 1;

    while((tmp /= 10) > 0)
        len ++;

    val_str = (char *) pvPortMalloc(len + 1);
    sprintf(val_str, "%d", value);

    return val_str;
}

static void dtls_psk_client_handler(void)
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

    int dtls_ciphersuites[3];

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold( DEBUG_LEVEL );
#endif

    mbedtls_platform_set_calloc_free(my_calloc, vPortFree);

    /*
     * 0. Initialize the RNG and the session data
     */
    mbedtls_net_init( &server_fd );
    mbedtls_ssl_init( &ssl );
    mbedtls_ssl_config_init( &conf );
    mbedtls_x509_crt_init( &cacert );
    mbedtls_ctr_drbg_init( &ctr_drbg );

    mbedtls_printf( "\n  . Seeding the random number generator..." );
//    fflush( stdout );

    mbedtls_entropy_init( &entropy );
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *) pers,
                               strlen( pers ) ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\n" );


    printf( "\n  . Loading the psk and psk_identity" );
//    fflush( stdout );

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
//    fflush( stdout );

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
//    fflush( stdout );

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
    // mbedtls_ssl_conf_authmode( &conf, MBEDTLS_SSL_VERIFY_OPTIONAL );
    // mbedtls_ssl_conf_ca_chain( &conf, &cacert, NULL );
    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );
//    mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );

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

    mbedtls_ssl_set_bio( &ssl, &server_fd,
                         mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout );

    mbedtls_ssl_set_timer_cb( &ssl, &timer, mbedtls_timing_set_delay,
                                            mbedtls_timing_get_delay );

    mbedtls_printf( " ok\n" );

    /*
     * 4. Handshake
     */
    mbedtls_printf( "  . Performing the SSL/TLS handshake..." );
//    fflush( stdout );

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
//    fflush( stdout );

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
//    fflush( stdout );

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

    /* Shell can not handle large exit numbers -> 1 for errors */
    if( ret < 0 )
        ret = 1;

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