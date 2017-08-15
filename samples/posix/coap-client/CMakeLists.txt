cmake_minimum_required(VERSION 2.6)
project("dtls-psk")

# gdb debug   $ gdbtui dtls_server
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# targets
set(targets 
    coap_client
)

# include file
include_directories(../../../external/coap)
add_definitions(-DCMAKE_BUILD_TYPE=Debug)

# source file
aux_source_directory(../../../external/coap COAP_LIB_SRC)
add_library(coap ${COAP_LIB_SRC})

# lib
set(libs 
    coap
)

# executable file
add_executable(coap_client coap_client.c)
target_link_libraries(coap_client ${libs})

