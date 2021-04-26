cmake_minimum_required(VERSION 3.17)
project(montecarlo)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(montecarlo
        barrier.cpp
        barrier.h)
