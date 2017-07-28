## samples/stm32/xx-demo
应用示例，包括用户示例代码和配置文件。配置文件中通常包括 硬件配置、mbedtls配置、rtos配置、lwip配置、coap配置。
```
xx-demo
├── config
│   ├── FreeRTOSConfig.h        //rtos配置
│   ├── lwipopts.h              // lwip配置
│   ├── mbedtls_config.h        // mbedtls配置
│   └── stm32f7xx_hal_conf.h    // 硬件配置
├── xx.c                        // 应用源文件
├── xx.h                        // 应用头文件
└── READNE.md                   // 说明文件
```