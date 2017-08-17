cmake_minimum_required(VERSION 2.6)
project("lwm2m_test")

# gdb debug   $ gdbtui dtls_server
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# targets
set(targets 
    lwm2m_client
)

# include file
include_directories(./ ../../../../../external/wakaama ../../../../../external/wakaama/er-coap-13)
include_directories(../../../../../external/mbedtls/include)

add_definitions(-DCMAKE_BUILD_TYPE=Debug -DLWM2M_LITTLE_ENDIAN -DLWM2M_CLIENT_MODE -DLWM2M_BOOTSTRAP -DWITH_MBEDTLS -DLWM2M_MEMORY_TRACE)

# source file
aux_source_directory(../../../../../external/wakaama WAKAAMA_LIB_SRC)
aux_source_directory(../../../../../external/wakaama/er-coap-13 WAKAAMA_LIB_SRC)
add_library(wakaama ${WAKAAMA_LIB_SRC})

aux_source_directory(../../../../../external/mbedtls/library MBEDTLS_LIB_SRC)
add_library(mbedtls ${MBEDTLS_LIB_SRC})

# lib
set(libs 
    wakaama
    mbedtls
)

SET(SOURCES
    commandline.c
    connection.c
    lwm2m_client_main.c
    object_access_control.c
    object_connectivity_moni.c
    object_connectivity_stat.c
    object_device.c
    object_firmware.c
    object_location.c
    object_security.c
    object_server.c
    platform.c
    test_object.c
    easy_tls.c
)

# executable file
add_executable(lwm2m_client ${SOURCES} lwm2m_client_main.c)
target_link_libraries(lwm2m_client ${libs})
