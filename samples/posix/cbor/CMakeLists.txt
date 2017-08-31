cmake_minimum_required(VERSION 3.0)
project("tinycbor")

set(libs tinycbor)

set(targets demo)

add_executable(demo demo.c)
target_link_libraries(demo ${libs})
