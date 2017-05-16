cmake_minimum_required(VERSION 2.6)
project("hash")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    md5
    sha256
)

add_executable(md5 md5.c)
target_link_libraries(md5 ${libs})

add_executable(sha256 sha256.c)
target_link_libraries(sha256 ${libs})
