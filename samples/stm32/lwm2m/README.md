## lwm2m
- IAR lwm2m client 调试通过，通过 LWM2M_CLIENT_WITH_DTLS 宏定义选择是否开启 dtls 支持.

## 待确认问题
- 确认 malloc / free 是否存在内存泄漏
- 确认 ipv4 / ipv6 支持情况
- 确认 client bootstrap server 代码是否完善
- 原 client 代码中 bind（create_socket 接口）操作的意义？

## 待办事项
- 整理 lwm2m 代码，设计 lwm2m 示例