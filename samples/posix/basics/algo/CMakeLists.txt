cmake_minimum_required(VERSION 2.6)
project("algo")

set(targets
    exgcd
    gen_tables
)

add_executable(exgcd exgcd.c)
target_link_libraries(exgcd)

add_executable(gen_tables gen_tables.c)
target_link_libraries(gen_tables)

