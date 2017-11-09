cmake_minimum_required(VERSION 2.6)
project("bignum")

set(libs
	mbedtls
    mbedcrypto
)

set(targets
    gcd
    operate
    mpi-demo
)

add_executable(gcd gcd.c)
target_link_libraries(gcd ${libs})

add_executable(operate operate.c)
target_link_libraries(operate ${libs})

add_executable(mpi-demo mpi-demo.c)
target_link_libraries(mpi-demo ${libs})

