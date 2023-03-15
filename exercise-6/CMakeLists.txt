cmake_minimum_required(VERSION 3.24)
project(exercise_5)

set(CMAKE_CXX_STANDARD 17)

set(OPENSSL_ROOT_DIR /usr/local/openssl-3.0.8)
set(OPENSSL_CRYPTO_LIBRARY "/usr/local/openssl-3.0.8/libcrypto.dylib;/usr/local/openssl-3.0.8/libssl.dylib")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})


add_executable(exercise_5 WebServer.cpp Client.cpp)
target_link_libraries(exercise_5 ${OPENSSL_LIBRARIES})
