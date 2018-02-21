
set(libs mbedtls)
add_executable(ciphersuite-list ciphersuite-list.c)
target_link_libraries(ciphersuite-list ${libs})
