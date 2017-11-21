cmake_minimum_required(VERSION 2.6)
project("algo")

set(targets
    exgcd
    galois
)

add_executable(exgcd exgcd.c)
target_link_libraries(exgcd)

add_executable(galois galois.c)
target_link_libraries(galois)

