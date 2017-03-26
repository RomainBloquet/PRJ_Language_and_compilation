cmake_minimum_required(VERSION 3.5)
project(PRJ_LC___33)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    F01fonctions.cpp
    F01grammaire1.txt
    F01grammaire2.txt
    F01grammaire3.txt
    F01grammaire4.txt
    F01grammaire5.txt
    F01grammaire6.txt
    F01grammaire7.txt
    F01grammaire8.txt
    F01grammaire9.txt
    F01header.h
    F01main.cpp
    F01nonRecur.txt
    PRJ_LC.cbp
    PRJ_LC.depend
    PRJ_LC.layout)

add_executable(PRJ_LC___33 ${SOURCE_FILES})