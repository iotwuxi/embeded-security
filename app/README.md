# 模块说明
> 可参考 ```dtls-demo``` 示例工程构建应用示例

## app/net 
网络模块，可完成网络环境初始化，用户示例需要调用 ```app_net_register_thread``` 接口注册用户任务，注册后用户示例任务会在网络环境初始化后执行。

## app/common
通用模块，包括mbedtls硬件替换接口和主程序入口，用户示例代码中需要提供以下接口： 
```
void bsp_init(void); //硬件初始化，如系统时钟、随机数等
void app_init(void); //示例应用初始化
```

## app/xx-demo
示例模块，包括用户示例代码和配置文件。配置文件中通常包括 硬件配置、mbedtls配置、rtos配置、lwip配置、coap配置。
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