cmake_minimum_required(VERSION 2.6)
project("md-list")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    md-list
)

add_executable(md-list md-list.c)
target_link_libraries(md-list ${libs})
