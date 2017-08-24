cmake_minimum_required(VERSION 2.6)
project("sym_cipher")

add_definitions(-std=c99)

set(libs
    mbedcrypto 
)

set(targets
    aes-cbc
    aes-ecb
)

add_executable(aes-ecb aes-ecb.c)
target_link_libraries(aes-ecb ${libs})

add_executable(aes-cbc aes-cbc.c)
target_link_libraries(aes-cbc ${libs})


