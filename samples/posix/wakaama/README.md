
## lwm2m

### coap
```
# server
$ ./lwm2m_server -p 1

# client
$ ./lwm2m_client -h coap://localhost
$ ./lwm2m_client -h coap://wsncoap.org
```

### coaps
```
# server
$ ./lwm2m_server -p 1 -i PSK_identity -s 11111111

# client
$ ./lwm2m_client -h coaps://localhost -i PSK_identity -s 11111111
$ ./lwm2m_client -h coaps://wsncoap.org -i PSK_identity -s 11111111
```

### 待办事项
- [ ] 考虑是否需要加入 ipv6 支持
- [ ] 整理并优化代码，已修复一处free bug(搜索 xianrenqiu)