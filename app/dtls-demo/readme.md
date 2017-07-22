# 本地mbedtls_config.h
stm32f7_mbedtls_config.h与mbedtls_config.h 存在差异
- comment #define MBEDTLS_FS_IO
- uncomment #define MBEDTLS_NO_PLATFORM_ENTROPY
- 疑问 #define MBEDTLS_SSL_MAX_FRAGMENT_LENGTH ??
- 疑问 #define MBEDTLS_DEBUG_C
- uncomment #define MBEDTLS_PLATFORM_C
- uncomment #define MBEDTLS_TIMING_C