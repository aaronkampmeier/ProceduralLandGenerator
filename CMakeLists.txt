cmake_minimum_required(VERSION 3.16)
project(Assignment_3)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

set(Assign_Sources kampmeier_aaron_hw3.cpp kampmeier_aaron_LandGeneration.cpp kampmeier_aaron_LandGeneration.h)

# Add in the termcolor library
add_subdirectory(termcolor EXCLUDE_FROM_ALL)
#ADD_LIBRARY(termcolor INTERFACE IMPORTED)

add_executable(Assignment_3 ${Assign_Sources})
target_link_libraries(Assignment_3 PRIVATE termcolor::termcolor)