cmake_minimum_required(VERSION 2.6)
project("pkey")

set(libs
    mbedcrypto 
    mbedtls
)

set(targets
    dh_client
    dh_server
    dh_genprime
)

add_executable(dh_client dh_client.c)
target_link_libraries(dh_client ${libs})
add_executable(dh_server dh_server.c)
target_link_libraries(dh_server ${libs})
add_executable(dh_genprime dh_genprime.c)
target_link_libraries(dh_genprime ${libs})