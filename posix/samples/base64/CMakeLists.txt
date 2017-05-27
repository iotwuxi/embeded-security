cmake_minimum_required(VERSION 2.6)
project("base64-test")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    base64-encode-test
    base64-decode-test
)

add_executable(base64-encode-test base64-encode-test.c)
target_link_libraries(base64-encode-test ${libs})

add_executable(base64-decode-test base64-decode-test.c)
target_link_libraries(base64-decode-test ${libs})
