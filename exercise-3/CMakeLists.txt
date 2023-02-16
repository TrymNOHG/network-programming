cmake_minimum_required(VERSION 3.24)
project(exercise_3)

set(CMAKE_CXX_STANDARD 17)

add_executable(exercise_3 main.cpp SocketServer.h SocketServer.cpp SocketClient.h SocketClient.cpp)
