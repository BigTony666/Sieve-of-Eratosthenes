cmake_minimum_required(VERSION 2.8)
project(philosophers_dinning)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11)

set (CMAKE_CXX_FLAGS "-pthread")

add_executable(sieve_of_eratosthenes main.cpp)