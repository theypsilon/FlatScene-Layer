cmake_minimum_required ( VERSION 2.6.4 FATAL_ERROR )
project ( FLATSCENE C CXX )

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

include("${CMAKE_CURRENT_SOURCE_DIR}/CMake/HPHPFunctions.cmake")
include(CheckFunctionExists)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/flatscene)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/itemology)


