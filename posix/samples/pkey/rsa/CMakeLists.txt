cmake_minimum_required(VERSION 2.6)
project("rsa")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    rsa_genkey
    rsa_encrypt
    rsa_decrypt
    rsa_sign
    rsa_verify
    rsa_sign_pss
    rsa_verify_pss    
)

add_executable(rsa_genkey rsa_genkey.c)
target_link_libraries(rsa_genkey ${libs})

add_executable(rsa_encrypt rsa_encrypt.c)
target_link_libraries(rsa_encrypt ${libs})

add_executable(rsa_decrypt rsa_decrypt.c)
target_link_libraries(rsa_decrypt ${libs})

add_executable(rsa_sign rsa_sign.c)
target_link_libraries(rsa_sign ${libs})

add_executable(rsa_verify rsa_verify.c)
target_link_libraries(rsa_verify ${libs})

add_executable(rsa_sign_pss rsa_sign_pss.c)
target_link_libraries(rsa_sign_pss ${libs})

add_executable(rsa_verify_pss rsa_verify_pss.c)
target_link_libraries(rsa_verify_pss ${libs})