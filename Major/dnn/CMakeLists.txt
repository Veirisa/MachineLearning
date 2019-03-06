cmake_minimum_required(VERSION 3.12)
project(dnn)

set(CMAKE_CXX_STANDARD 14)

add_executable(dnn dnn.cpp)
add_executable(generator generator.cpp)
add_executable(smart_generator smart_generator.cpp)