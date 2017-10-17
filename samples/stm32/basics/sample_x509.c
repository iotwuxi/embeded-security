#include "basics.h"

char buf[2048];

struct cert_list {
    const char* buf;
    size_t len;
};

int sample_x509(void)
{
    int ret;
    const char *cert_buf;
    int cert_len = 0;
    mbedtls_x509_crt cert;
    
    mbedtls_printf("\n==========X509 Sample=========\n");
    /* mbedtls中所有预定义证书 */
    struct cert_list list[] = {
        {mbedtls_test_ca_crt_ec, mbedtls_test_ca_crt_ec_len},
        {mbedtls_test_srv_crt_ec, mbedtls_test_srv_crt_ec_len},
        {mbedtls_test_cli_crt_ec, mbedtls_test_cli_crt_ec_len},
        {mbedtls_test_ca_crt, mbedtls_test_ca_crt_len},
        {mbedtls_test_srv_crt, mbedtls_test_srv_crt_len},
        {mbedtls_test_cli_crt, mbedtls_test_cli_crt_len},
        {NULL, 0}
    };
    
    for (int i = 0; list[i].buf != NULL; i++) {
        cert_buf = list[i].buf;
        cert_len = list[i].len;

        mbedtls_x509_crt_init(&cert);
        ret = mbedtls_x509_crt_parse(&cert, (const unsigned char *)cert_buf, cert_len);
        if (ret < 0) {
            mbedtls_printf("failed: -0x%04x\n", -ret);
            goto exit;
        }

        mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", &cert);
        mbedtls_printf("----------Cert----------\n");
        mbedtls_printf("Length: %d\n", cert_len);
        mbedtls_printf("%s", buf); 
    }

exit:
    mbedtls_x509_crt_free(&cert);
    
    return ret;
}