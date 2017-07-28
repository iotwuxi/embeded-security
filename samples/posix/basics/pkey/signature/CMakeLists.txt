cmake_minimum_required(VERSION 2.6)
project("pkey")

set(libs
    mbedcrypto 
)

set(targets
    rsa_sign
)

add_executable(rsa_sign rsa_sign.c)
target_link_libraries(rsa_sign ${libs})