cmake_minimum_required(VERSION 2.6)
project("encrypt")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    aes
    des
)

add_executable(aes aes.c)
target_link_libraries(aes ${libs})

add_executable(des des.c)
target_link_libraries(des ${libs})