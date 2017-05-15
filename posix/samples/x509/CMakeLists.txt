cmake_minimum_required(VERSION 2.6)
project("x509-info")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    x509-info
)

add_executable(x509-info x509-info.c)
target_link_libraries(x509-info ${libs})
