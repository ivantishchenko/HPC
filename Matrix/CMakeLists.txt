# -*- mode: cmake; -*-
# Time-stamp: <Tue 2015-10-27 14:51 svarrette>
################################################################################
# CMakeLists.txt --  [CMake](http://www.cmake.org/) Configuration
#                 ____ __  __       _        _     _     _
#                / ___|  \/  | __ _| | _____| |   (_)___| |_ ___
#               | |   | |\/| |/ _` | |/ / _ \ |   | / __| __/ __|
#               | |___| |  | | (_| |   <  __/ |___| \__ \ |_\__ \
#                \____|_|  |_|\__,_|_|\_\___|_____|_|___/\__|___/
#
################################################################################
cmake_minimum_required (VERSION 2.8.9)
project (MPI_Matrix C)

SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_FLAGS --std=c99 )

enable_testing()

#==== MPI
find_package(MPI REQUIRED)
set_property( DIRECTORY PROPERTY COMPILE_DEFINITIONS ${MPI_C_COMPILE_FLAGS} )
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} ${MPI_C_COMPILE_FLAGS}")
include_directories(SYSTEM ${MPI_C_INCLUDE_PATH})
list(APPEND EXTRALIBS ${MPI_C_LIBRARIES})

#==== Let's go
add_executable(matrix  matrix.c)
target_link_libraries(matrix  ${EXTRALIBS})