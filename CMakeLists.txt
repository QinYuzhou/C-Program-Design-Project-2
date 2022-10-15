cmake_minimum_required (VERSION 3.2)

project(Project2)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

add_executable(cal C++Project2.cpp BigNumber.cpp FFT.cpp)