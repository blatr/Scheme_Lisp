cmake_minimum_required(VERSION 3.14)
project(Scheme_Lisp)

set(CMAKE_CXX_STANDARD 17)

add_library(libscheme
        parser.cpp
        scheme.cpp)

add_executable(Scheme_Lisp main.cpp)

target_link_libraries(Scheme_Lisp
        libscheme)