### Building and Running (DTLS)
*******************************

command

```
cd posix
make
```

run server application

```
./coaps_server
```

You will get the following output

```
. Loading the server cert. and key... ok
. Bind on udp/*/4433 ... ok
. Seeding the random number generator... ok
. Setting up the DTLS data... ok
. Waiting for a remote connection ...
```

run client application in other terminal window:
	
```
./coaps_client
```

You will get the following output

```
. Seeding the random number generator... ok
. Loading the CA root certificate ... ok (0 skipped)
. Connecting to udp/localhost/4433... ok
. Setting up the DTLS structure... ok
. Performing the SSL/TLS handshake... ok
. Performing the DTLS handshake... hello verification requested
. Waiting for a remote connection ... ok
. Performing the DTLS handshake... ok
. Verifying peer X.509 certificate... ok
> Write to server: 9 bytes written

Echo this
  < Read from server: ok
  < Read from client: 9 bytes read

Echo this
  > Write to client: 9 bytes read

Echo this
  . Closing the connection... done
  9 bytes written

Echo this
    . Closing the connection... done
    . Waiting for a remote connection ...
```

If the server does not receive the  messages, restart the app and try to connect the client again.