cmake_minimum_required(VERSION 2.6)
project("x509_info")

# gdb debug   $ gdbtui dtls_server
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# targets
set(targets 
	x509-demo
)

# include file
include_directories(./ ../../../../external/mbedtls/include)
add_definitions(-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h> -DCMAKE_BUILD_TYPE=Debug)

# source file
aux_source_directory(../../../../external/mbedtls/library MBEDTLS_LIB_SRC)
add_library(mbedtls ${MBEDTLS_LIB_SRC})

# lib
set(libs 
    mbedtls
)

# executable file
add_executable(x509_info x509_info.c)
target_link_libraries(x509_info ${libs})



