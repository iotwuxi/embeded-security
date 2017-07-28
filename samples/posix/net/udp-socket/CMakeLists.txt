cmake_minimum_required(VERSION 2.6)
project("udp-socket")

set(targets
	server
	client
)

add_executable(client client.c)
add_executable(server server.c)

