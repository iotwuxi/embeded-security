cmake_minimum_required(VERSION 2.6)
project("self_test")

# gdb debug   $ gdbtui self_test
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# targets
set(targets 
    self_test 
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
add_executable(self_test self_test.c ../net_sockets.c)
target_link_libraries(self_test ${libs})




