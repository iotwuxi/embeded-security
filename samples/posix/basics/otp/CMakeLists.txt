cmake_minimum_required(VERSION 2.6)
project("otp")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    otp
)

add_executable(otp main.c otp.c)
target_link_libraries(otp ${libs})
