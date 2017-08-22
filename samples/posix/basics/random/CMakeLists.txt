cmake_minimum_required(VERSION 2.6)
project("random")

set(libs
    mbedcrypto 
)

set(targets ctr_drbg hmac_drbg)

add_executable(ctr_drbg ctr_drbg.c)
target_link_libraries(ctr_drbg ${libs})

add_executable(hmac_drbg hmac_drbg.c)
target_link_libraries(hmac_drbg ${libs})
