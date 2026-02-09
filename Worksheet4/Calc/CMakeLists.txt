cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(calc)

# This project contains a library
add_library(maths adder.cpp)

# If static libs are compiled we need to communicate that to the source code.
# This definition controls how MATHSLIBAPI is defined in adder.h
if (NOT BUILD_SHARED_LIBS)
  add_definitions(-DmathsSTATIC)
endif()

# It also contains an executable
add_executable(calc calc.cpp)

# The executable uses the library
target_link_libraries(calc maths)
