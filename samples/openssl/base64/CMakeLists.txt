cmake_minimum_required(VERSION 3.8)
project("Base64")

set(LIBS 
	ssl crypto)

set(SOURCES 
	${CMAKE_CURRENT_LIST_DIR}/base64.c)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

