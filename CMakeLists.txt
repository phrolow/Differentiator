cmake_minimum_required(VERSION 3.23)
project(Differentiator)

set(CMAKE_CXX_STANDARD 14)

add_executable(Differentiator main.cpp Tree/tree.cpp Tree/node.cpp Text/text.cpp Expressions/differentiator.cpp Text/reader.cpp Debug/debug.cpp Expressions/simplification.cpp Text/output.cpp)
