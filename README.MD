## CoAP over DTLS sample

Overview
****************
This sample code shows a CoAP over DTLS client using mbedTLS on Linux or Nucleo_f411re.

![image](https://bitbucket.org/iotwuxi/embeded-security/src/38d168143ad2001cd5225c436715586110ccad37/image/coaps.jpg?at=master&fileviewer=file-view-default)

[Nucleo_f411re](http://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f411re.html)
- STM32F411RE microcontroller
- SPI master to communicate with other chips
- USART serial port with STLink USB connection for standard I/O
- JTAG with STLink USB connection to flash program

Ethernet
- Wiznet [W5100](https://detail.tmall.com/item.htm?spm=a220m.1000858.1000725.6.PfIFVz&id=38628268194&areaId=320200&user_id=1639117447&cat_id=2&is_b=1&rn=0b30835c19ec2589b96cf12ba7118ffd) that implements the TCP/IP and UDP/IP stacks.
- SPI connection to the board below.

Library
- [Mbedtls](https://github.com/ARMmbed/mbedtls) 
- coap
- JSON
- w5x00

TODO List
- [x] DTLS Server on linux              
- [x] DTLS Client on linux              
- [ ] COAPS Server on linux             
- [ ] COAPS Client on linux             
- [ ] COAPS Client on Nucleo_f411re
