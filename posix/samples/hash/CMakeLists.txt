cmake_minimum_required(VERSION 2.6)
project("md5")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    md5
)

add_executable(md5 md5.c)
target_link_libraries(md5 ${libs})
