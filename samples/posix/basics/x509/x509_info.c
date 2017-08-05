
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
#define mbedtls_time_t     time_t
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"


int main(void)
{
    int ret;
    char buf[4096];
    const char *cert_buf;
    int cert_len = 0;
    mbedtls_x509_crt cert;

    /* */
    cert_buf = mbedtls_test_ca_crt_ec;
    cert_len = mbedtls_test_ca_crt_ec_len;

    mbedtls_x509_crt_init(&cert);
    ret = mbedtls_x509_crt_parse(&cert, (const unsigned char *)cert_buf, cert_len);
    if (ret < 0) {
        mbedtls_printf("failed: %dX\n", ret);
        goto exit;
    }

    mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", &cert);
    mbedtls_printf("----------Cert----------\n");
    mbedtls_printf("Length: %d\n", cert_len);
    mbedtls_printf("%s", buf);


exit:
    mbedtls_x509_crt_free(&cert);

    return 0;
}

