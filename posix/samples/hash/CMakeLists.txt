cmake_minimum_required(VERSION 2.6)
project("hash")

set(libs
    mbedcrypto 
)

set(targets
    sha256
)

add_executable(sha256 sha256.c)
target_link_libraries(sha256 ${libs})
