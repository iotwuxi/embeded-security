## coaps client 示例代码
- mbedtls
- [PicoCoap](https://github.com/exosite-garage/PicoCoAP)

```
# 开启 coaps server (psk 方式)
$ cd <aliyun>/coaps-works/node-coap-dtls/examples
$ node dtlsServer.js         

# 运行 coaps client
$ cmake .
$ make
$ ./coaps_client wsncoap.org 5684  

	. Seeding the random number generator... ok

	  . Loading the psk and psk_identity ok
	  . Connecting to udp/wsncoap.org/5684... ok
	  . Setting up the DTLS structure... ok
	  . Performing the SSL/TLS handshake... ok
	  . Verifying peer ... ok
	  > Write to server: 33 bytes written

	this is coaps client.

	  < Read from server: 18 bytes read

	bE▒Vx▒Hello test


	[POST] coaps:wsncoap.org:5684/test pyload:Hello test


	  . Closing the connection... done

```