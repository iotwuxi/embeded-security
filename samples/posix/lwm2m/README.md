## lwm2m

### leshan
```
# server
http://leshan.eclipse.org

# client
$ ./lwm2m_client -h coap://leshan.eclipse.org -p 5683
$ ./lwm2m_client -h coaps://leshan.eclipse.org -p 5684 -i root -s 123456
```

### example
```
# server
$ ./lwm2m_server -p 0
$ ./lwm2m_server -p 1 -i PSK_identity -s 11111111

# client
$ ./lwm2m_client -h coap://wsncoap.org -p 6683
$ ./lwm2m_client -h coaps://wsncoap.org -p 6684 -i PSK_identity -s 11111111
```

