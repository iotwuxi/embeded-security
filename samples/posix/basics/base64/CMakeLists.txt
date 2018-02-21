project("Base64")
set(libs mbedcrypto)

add_executable(base64 base64.c)
target_link_libraries(base64 ${libs})

