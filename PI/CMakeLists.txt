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
project (MPI_CircuitSatisfiability C)

SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_FLAGS --std=c99 )

enable_testing()

#==== MPI
find_package(MPI REQUIRED)
set_property( DIRECTORY PROPERTY COMPILE_DEFINITIONS ${MPI_C_COMPILE_FLAGS} )
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} ${MPI_C_COMPILE_FLAGS}")
include_directories(SYSTEM ${MPI_C_INCLUDE_PATH})
list(APPEND EXTRALIBS ${MPI_C_LIBRARIES})

#==== PI 
add_executable(pi_1  src/pi_1.c)
target_link_libraries(pi_1     ${EXTRALIBS})

add_executable(pi_2  src/pi_2.c)
target_link_libraries(pi_2     ${EXTRALIBS})

add_executable(pi_3  src/pi_3.c)
target_link_libraries(pi_3     ${EXTRALIBS})

#==== Simpson
add_executable(sim_1  src/sim_1.c)
target_link_libraries(sim_1     ${EXTRALIBS})

add_executable(sim_2  src/sim_2.c)
target_link_libraries(sim_2     ${EXTRALIBS})

add_executable(sim_3  src/sim_3.c)
target_link_libraries(sim_3     ${EXTRALIBS})

#==== Monte carlo
add_executable(mc_1  src/mc_1.c)

add_executable(mc_3 src/mc_3.c)
target_link_libraries(mc_3     ${EXTRALIBS})