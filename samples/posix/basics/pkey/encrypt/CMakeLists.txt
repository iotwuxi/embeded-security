cmake_minimum_required(VERSION 2.6)
project("pkey")

set(libs
    mbedcrypto 
)

set(targets
    rsa_enc
)

add_executable(rsa_enc rsa_enc.c)
target_link_libraries(rsa_enc ${libs})