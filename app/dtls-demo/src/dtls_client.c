#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#define DFL_PSK                 "password"
#define DFL_PSK_IDENTITY        "identity"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf     printf
#define mbedtls_fprintf    fprintf
#endif

#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/timing.h"

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"


#include "main.h"
#include "cmsis_os.h"

#include <string.h>

#define DTLS_CLIENT_THREAD_PRIO      (osPriorityRealtime)

#define SERVER_PORT "4433"
#define SERVER_NAME "aliyun"
#define SERVER_ADDR "139.196.187.107" /* forces IPv4 */
#define MESSAGE     "Hello DTLS Server"

#define READ_TIMEOUT_MS 1000
#define MAX_RETRY       5

struct dtls_timing_context 
{
    uint32_t snapshot;
    uint32_t int_ms;
    uint32_t fin_ms;
};

uint32_t timer_get_32(void)
{
    uint32_t ticks = osKernelSysTick();
    return ((1000 * ticks) / osKernelSysTickFrequency);
}

void dtls_timing_set_delay(void *data, uint32_t int_ms, uint32_t fin_ms)
{
    struct dtls_timing_context *ctx = (struct dtls_timing_context *)data;

    ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;

    if (fin_ms != 0) {
        ctx->snapshot = timer_get_32();
    }
}

int dtls_timing_get_delay(void *data)
{
    struct dtls_timing_context *ctx = (struct dtls_timing_context *)data;
    unsigned long elapsed_ms;

    if (ctx->fin_ms == 0) {
        return -1;
    }

    elapsed_ms = timer_get_32() - ctx->snapshot;

    if (elapsed_ms >= ctx->fin_ms) {
        return 2;
    }

    if (elapsed_ms >= ctx->int_ms) {
        return 1;
    }

    return 0;
}

int dtls_client_thread(void* args)
{   
      int ret, len;
      mbedtls_net_context server_fd;
      unsigned char buf[1024];
      const char *pers = "dtls_client";
      int retry_left = MAX_RETRY;

      mbedtls_entropy_context entropy;
      mbedtls_ctr_drbg_context ctr_drbg;
      mbedtls_ssl_context ssl;
      mbedtls_ssl_config conf;
      mbedtls_timing_delay_context timer;

      int dtls_ciphersuites[3];

      /*
       * 0. Initialize the RNG and the session data
       */
      mbedtls_net_init( &server_fd );
      mbedtls_ssl_init( &ssl );
      mbedtls_ssl_config_init( &conf );
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


      printf( "\n  . Loading the psk and psk_identity" );
      fflush( stdout );

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
      mbedtls_printf( "  . Connecting to [udp] %s:%s...", SERVER_ADDR, SERVER_PORT );
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
      mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );

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

      mbedtls_ssl_set_timer_cb( &ssl, &timer, dtls_timing_set_delay,
                                              dtls_timing_get_delay );

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
      if( mbedtls_ssl_get_verify_result( &ssl ) != 0 )
      {
          mbedtls_printf( " failed\n" );
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
      mbedtls_ssl_free( &ssl );
      mbedtls_ssl_config_free( &conf );
      mbedtls_ctr_drbg_free( &ctr_drbg );
      mbedtls_entropy_free( &entropy );

      printf("dtls client thread exit.\n");
      
      for(;;)
      {
           
      }
}

/**
* @brief  初始化UDP客户端
*/
void dtls_client_init()
{
    sys_thread_new("DTLS Client", (lwip_thread_fn)dtls_client_thread, NULL, DEFAULT_THREAD_STACKSIZE, DTLS_CLIENT_THREAD_PRIO);
}
