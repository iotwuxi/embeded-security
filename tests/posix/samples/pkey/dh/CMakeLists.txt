cmake_minimum_required(VERSION 2.6)
project("pkey")

set(libs
    mbedcrypto 
)

set(targets
    ecdh
)

add_executable(ecdh ecdh.c)
target_link_libraries(ecdh ${libs})