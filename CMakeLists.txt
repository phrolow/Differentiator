cmake_minimum_required(VERSION 3.23)
project(Differentiator)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS  "-O0")

add_executable(Differentiator main.cpp tree.cpp node.cpp text.cpp differentiator.cpp reader.cpp debug.cpp simplification.cpp output.cpp)
