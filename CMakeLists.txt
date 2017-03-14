cmake_minimum_required(VERSION 2.6)
project(ethercatstack)

add_executable(ethercatstack main.cpp)

install(TARGETS ethercatstack RUNTIME DESTINATION bin)
