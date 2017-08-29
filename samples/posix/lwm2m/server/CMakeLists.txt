cmake_minimum_required(VERSION 2.6)
project("lwm2m_server")

# gdb debug   $ gdbtui dtls_server
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# targets
set(targets 
    lwm2m_server
)

# include file
include_directories(./inc) 
include_directories(../../../../app/lwm2m/)
include_directories(../../../../external/wakaama)
include_directories(../../../../external/wakaama/er-coap-13)
include_directories(../../../../external/mbedtls/include)

#define 
add_definitions(-DCMAKE_BUILD_TYPE=Debug)
add_definitions(-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h>)
add_definitions(-DLWM2M_LITTLE_ENDIAN -DLWM2M_SERVER_MODE -DLWM2M_BOOTSTRAP -DWITH_MBEDTLS)

# source file
aux_source_directory(../../../../external/wakaama WAKAAMA_LIB_SRC)
aux_source_directory(../../../../external/wakaama/er-coap-13 WAKAAMA_LIB_SRC)
add_library(wakaama ${WAKAAMA_LIB_SRC})

aux_source_directory(../../../../external/mbedtls/library MBEDTLS_LIB_SRC)
add_library(mbedtls ${MBEDTLS_LIB_SRC})

# lib
set(libs 
    wakaama
    mbedtls
)

SET(SOURCES
    ../../../../app/lwm2m/commandline.c
    ../../../../app/lwm2m/object_access_control.c
    ../../../../app/lwm2m/object_connectivity_moni.c
    ../../../../app/lwm2m/object_connectivity_stat.c
    ../../../../app/lwm2m/object_device.c
    ../../../../app/lwm2m/object_firmware.c
    ../../../../app/lwm2m/object_location.c
    ../../../../app/lwm2m/object_security.c
    ../../../../app/lwm2m/object_server.c
    ../../../../app/lwm2m/test_object.c
    ../../../../app/lwm2m/easy_tls.c
)

SET(SOURCES
    ${SOURCES}
    lwm2m_platform.c
    lwm2m_connection.c
    lwm2m_server_main.c
)

# executable file
add_executable(lwm2m_server ${SOURCES})
target_link_libraries(lwm2m_server ${libs})
