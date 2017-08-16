cmake_minimum_required(VERSION 2.6)
project("lightclient")

# gdb debug   $ gdbtui dtls_server
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# targets
set(targets 
    lightclient
)

# include file
include_directories(./ ../../../../external/wakaama ../../../../external/wakaama/er-coap-13)
include_directories(../shared)
add_definitions(-DCMAKE_BUILD_TYPE=Debug -DLWM2M_LITTLE_ENDIAN -DLWM2M_CLIENT_MODE)

# source file
aux_source_directory(../../../../external/wakaama WAKAAMA_LIB_SRC)
aux_source_directory(../../../../external/wakaama/er-coap-13 WAKAAMA_LIB_SRC)
add_library(wakaama ${WAKAAMA_LIB_SRC})

# lib
set(libs 
    wakaama
)

SET(SOURCES
    lightclient.c
    object_security.c
    object_server.c
    object_device.c
    test_object.c
    ../shared/commandline.c
    ../shared/connection.c
    ../shared/memtrace.c
    ../shared/platform.c
)

# executable file
add_executable(lightclient ${SOURCES})
target_link_libraries(lightclient ${libs})

