cmake_minimum_required(VERSION 3.24)
project(exercise_4)

set(CMAKE_CXX_STANDARD 17)

add_executable(exercise_4 UDPSocketClient.cpp UDPSocketServer.cpp)
