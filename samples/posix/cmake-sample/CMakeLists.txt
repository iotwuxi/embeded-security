cmake_minimum_required(VERSION 3.8)
project("cmake-sample")

# gdb debug
# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 在bashrc文件中定义环境变量 MBEDTLS_BASE 该变量保存mbedtls源代码路径

# 头文件查找路径
include_directories(./ $ENV{MBEDTLS_BASE}/include)
# 定义mbedtls配置文件
add_definitions(-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h>)

# 增加mbedtls源文件
aux_source_directory($ENV{MBEDTLS_BASE}/library mbedtls_src)

# 可执行文件
add_executable(ciphersuite-list  ciphersuite-list.c ${mbedtls_src}) 

