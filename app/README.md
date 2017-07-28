# 模块说明
> 可参考 ```samples/stm32/dtls-demo``` 示例工程构建应用示例

## app/main.c
主程序入口，用户示例代码中需要提供以下接口： 
```
void bsp_init(void); //硬件初始化，如系统时钟、随机数等
void app_init(void); //示例应用初始化
```
## app/mbedtls_alt.c
mbedtls 硬件替换接口，包括 timer、RNG 模块的替换.

## app/net 
网络模块，可完成网络环境初始化，用户示例需要调用 ```app_net_register_thread``` 接口注册用户任务，注册后用户示例任务会在网络环境初始化后执行。
