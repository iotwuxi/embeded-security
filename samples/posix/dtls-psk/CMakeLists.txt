cmake_minimum_required(VERSION 2.6)
project("dtls-psk")

# gdb debug   $ gdbtui dtls_server
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# targets
set(targets 
    dtls_psk_server 
    dtls_psk_client
)

# include file
include_directories(./ ../../../external/mbedtls/include)
add_definitions(-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h> -DCMAKE_BUILD_TYPE=Debug)

# source file
aux_source_directory(../../../external/mbedtls/library MBEDTLS_LIB_SRC)
add_library(mbedtls ${MBEDTLS_LIB_SRC})

# lib
set(libs 
    mbedtls
)

# executable file
add_executable(dtls_psk_server dtls_psk_server.c)
target_link_libraries(dtls_psk_server ${libs})

add_executable(dtls_psk_client dtls_psk_client.c)
target_link_libraries(dtls_psk_client ${libs})
