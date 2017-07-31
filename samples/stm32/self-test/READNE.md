# dtls-demo 示例代码
示例工程可作为 `mbedtls` 应用构建模板

## 任务优先级及堆栈占用情况
- DHCP          3(normal)    256 Byte
- blink         3(normal)    256 Byte
- DTLS Clint    3(normal)    2048 Byte
- TCP/IP        5(High)      1000 Byte
- Ethif         6(Realtime)  350 Byte

## 密码套件
```
MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256   
MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
```