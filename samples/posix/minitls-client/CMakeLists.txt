cmake_minimum_required(VERSION 3.8)
project("mini_client")

# gdb debug
# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 在bashrc文件中定义环境变量 MBEDTLS_BASE 该变量保存mbedtls源代码路径

# 头文件查找路径
include_directories(./ $ENV{MBEDTLS_BASE}/include)
include_directories(./ $ENV{MBEDTLS_BASE}/configs)

# 定义mbedtls配置文件
# add_definitions(-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h>)


# 增加mbedtls源文件
aux_source_directory($ENV{MBEDTLS_BASE}/library MBEDTLS_SOURCES)

set(SOURCES 
	${CMAKE_CURRENT_LIST_DIR}/mini_client.c 
	${MBEDTLS_SOURCES})
# 可执行文件
add_executable(${PROJECT_NAME}  ${SOURCES}) 

