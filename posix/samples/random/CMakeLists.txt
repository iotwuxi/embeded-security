cmake_minimum_required(VERSION 2.6)
project("random")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
    gen_entropy
    gen_random_ctr_drbg
)

add_executable(gen_entropy gen_entropy.c)
target_link_libraries(gen_entropy ${libs})

add_executable(gen_random_ctr_drbg gen_random_ctr_drbg.c)
target_link_libraries(gen_random_ctr_drbg ${libs})
