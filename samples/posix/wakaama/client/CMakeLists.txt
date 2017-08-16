cmake_minimum_required(VERSION 2.6)
project("lwm2mclient")

# gdb debug   $ gdbtui dtls_server
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# targets
set(targets 
    lwm2mclient
)

# include file
include_directories(./ ../../../../external/wakaama ../../../../external/wakaama/er-coap-13)
include_directories(../shared)
add_definitions(-DCMAKE_BUILD_TYPE=Debug -DLWM2M_LITTLE_ENDIAN -DLWM2M_CLIENT_MODE -DLWM2M_BOOTSTRAP)

# source file
aux_source_directory(../../../../external/wakaama WAKAAMA_LIB_SRC)
aux_source_directory(../../../../external/wakaama/er-coap-13 WAKAAMA_LIB_SRC)
add_library(wakaama ${WAKAAMA_LIB_SRC})

# lib
set(libs 
    wakaama
)

SET(SOURCES
    lwm2mclient.c
    lwm2mclient.h
    system_api.c
    object_security.c
    object_server.c
    object_device.c
    object_firmware.c
    object_location.c
    object_connectivity_moni.c
    object_connectivity_stat.c
    object_access_control.c
    test_object.c
    ../shared/commandline.c
    ../shared/connection.c
    ../shared/memtrace.c
    ../shared/platform.c
)

# executable file
add_executable(lwm2mclient ${SOURCES})
target_link_libraries(lwm2mclient ${libs})

