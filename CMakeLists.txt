cmake_minimum_required(VERSION 3.13)
project(sieve_of_eratosthenes)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(sieve_of_eratosthenes main.cpp)