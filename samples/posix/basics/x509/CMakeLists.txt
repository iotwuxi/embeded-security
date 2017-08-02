cmake_minimum_required(VERSION 2.6)
project("x509-info")

set(libs
    mbedtls
    mbedcrypto 
    mbedx509
)

set(targets
	x509_info
    gen_key
    cert_req
    cert_write
    cert_app
)

add_executable(cert_req cert_req.c)
target_link_libraries(cert_req ${libs})

add_executable(cert_write cert_write.c)
target_link_libraries(cert_write ${libs})


add_executable(gen_key gen_key.c)
target_link_libraries(gen_key ${libs})

add_executable(cert_app  cert_app.c)
target_link_libraries(cert_app ${libs})

add_executable(x509_info x509_info.c)
target_link_libraries(x509_info ${libs})
