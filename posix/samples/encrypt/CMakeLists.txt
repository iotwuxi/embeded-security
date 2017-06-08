cmake_minimum_required(VERSION 2.6)
project("encrypt")

set(libs
    mbedcrypto 
)

set(targets
    aes
    des
    aes_cbc_demo
)

add_executable(aes aes.c)
target_link_libraries(aes ${libs})

add_executable(des des.c)
target_link_libraries(des ${libs})

add_executable(aes_cbc_demo aes_cbc_demo.c)
target_link_libraries(aes_cbc_demo ${libs})
