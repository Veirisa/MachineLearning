cmake_minimum_required(VERSION 3.12)
project(other)

set(CMAKE_CXX_STANDARD 14)

add_executable(chi2 chi2.cpp)
add_executable(corr corr.cpp)
add_executable(dist dist.cpp)
add_executable(entr entr.cpp)
add_executable(rank rank.cpp)
add_executable(var var.cpp)
