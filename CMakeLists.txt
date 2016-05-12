cmake_minimum_required (VERSION 3.5.2)
project (AsciiChat)

add_executable(ac-server server.c)
add_executable(ac-client client.c)

install(TARGETS ac-server DESTINATION bin)
install(TARGETS ac-client DESTINATION bin)
