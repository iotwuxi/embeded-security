cmake_minimum_required(VERSION 2.6)
project("hmac")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    hmac
)

add_executable(hmac hmac.c)
target_link_libraries(hmac ${libs})
