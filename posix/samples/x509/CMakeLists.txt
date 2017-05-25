cmake_minimum_required(VERSION 2.6)
project("x509-info")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
#    x509-info
    gen_key
    cert_req
    cert_write
)

add_executable(cert_req cert_req.c)
target_link_libraries(cert_req ${libs})

add_executable(cert_write cert_write.c)
target_link_libraries(cert_write ${libs})


add_executable(gen_key gen_key.c)
target_link_libraries(gen_key ${libs})

#add_executable(x509-info x509-info.c)
#target_link_libraries(x509-info ${libs})

