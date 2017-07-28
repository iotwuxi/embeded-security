cmake_minimum_required(VERSION 2.6)
project("udp-socket")

set(targets
	udp_server
	udp_client
)

add_executable(udp_client udp_client.c)
add_executable(udp_server udp_server.c)

