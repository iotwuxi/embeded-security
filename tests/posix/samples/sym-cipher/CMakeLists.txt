cmake_minimum_required(VERSION 2.6)
project("sym_cipher")

set(libs
    mbedcrypto 
)

set(targets
    aes
)

add_executable(aes aes.c)
target_link_libraries(aes ${libs})
