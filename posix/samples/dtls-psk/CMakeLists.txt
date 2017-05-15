cmake_minimum_required(VERSION 2.6)
project("dtls-psk")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    dtls-psk-server
    dtls-psk-client
)

add_executable(dtls-psk-server dtls-psk-server.c)
target_link_libraries(dtls-psk-server ${libs})

add_executable(dtls-psk-client dtls-psk-client.c)
target_link_libraries(dtls-psk-client ${libs})