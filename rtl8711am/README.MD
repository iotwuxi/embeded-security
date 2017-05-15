## SSL over RTL8711AM sample   
****************************    

### Overview    
This sample code shows a CoAP over DTLS client using mbedTLS on RTL8711AM platform.    
![RTL8711AM.png](https://bitbucket.org/repo/9ppMAA6/images/2269939246-coaps.png)        

### Tools    
- IAR 7.30(Above)    
- mbedWinSerial_16466(driver for debug/uart)    
- iperf(test for net sample)    

### Component
- [Mbedtls](https://github.com/ARMmbed/mbedtls)    
- coap    
- mqtt    
- lwip    
- freertos    
- Cjson    

### TODO List
- [x] TLS Client on RTL8711AM    
- [ ] DTLS Client on RTL8711AM    
- [ ] DTLS PSK Client on RTL8711AM    
- [ ] COAPS Client on RTL8711AM    
- [ ] MQTTS Client on RTL8711AM    