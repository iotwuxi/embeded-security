cmake_minimum_required(VERSION 2.6)
project("base64")

set(libs
    mbedcrypto
)

set(targets
    base64
)

add_executable(base64 base64.c)
target_link_libraries(base64 ${libs})

