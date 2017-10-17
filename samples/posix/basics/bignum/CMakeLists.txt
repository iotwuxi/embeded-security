cmake_minimum_required(VERSION 2.6)
project("bignum")

set(libs
	mbedtls
    mbedcrypto
)

set(targets
    gcd
)

add_executable(gcd gcd.c)
target_link_libraries(gcd ${libs})

