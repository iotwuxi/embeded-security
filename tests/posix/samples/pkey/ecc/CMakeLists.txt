cmake_minimum_required(VERSION 2.6)
project("ecc")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    ecc_genkey
    ecdh_curve25519
    ecdsa
)

add_executable(ecc_genkey ecc_genkey.c)
target_link_libraries(ecc_genkey ${libs})

add_executable(ecdh_curve25519 ecdh_curve25519.c)
target_link_libraries(ecdh_curve25519 ${libs})

add_executable(ecdsa ecdsa.c)
target_link_libraries(ecdsa ${libs})
