cmake_minimum_required(VERSION 3.8)
project("CCM_GCM")

set(LIBS 
	ssl crypto)

#set(SOURCES 
#	${CMAKE_CURRENT_LIST_DIR}/base64.c)

add_executable(CCM ${CMAKE_CURRENT_LIST_DIR}/aesccm.c)
target_link_libraries(CCM ${LIBS})

add_executable(GCM ${CMAKE_CURRENT_LIST_DIR}/aesgcm.c)
target_link_libraries(GCM ${LIBS})

