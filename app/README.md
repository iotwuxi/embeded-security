# 模块说明

## app/mbedtls_alt.c
mbedtls 硬件替换接口，包括 timer、RNG 模块的替换.

## app/main.c
主程序入口，用户示例代码中需要提供以 `void sample_entry(void);` 接口，示例入口，可以为 `单独测试接口` 或 `任务接口`. 

## app/net 
网络模块，可完成网络环境初始化，用户示例(网络相关)需要调用 `app_net_register_thread` 接口注册用户任务，注册后用户示例任务会在网络环境初始化后执行。
