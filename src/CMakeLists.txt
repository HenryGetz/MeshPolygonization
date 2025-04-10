cmake_minimum_required(VERSION 3.10)

################################################################################
# Project Setup
################################################################################
project(MeshPolygonization CXX)

# Disallow in‑source builds (compare full source and binary directories)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed. Please create a separate build directory.")
endif()

################################################################################
# Set the C++ Standard
################################################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################
# Configure OpenMP
#
# On macOS, you might need to use Homebrew's libomp.
# These flags are appended to existing flags so as not to override them.
################################################################################
if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xpreprocessor -fopenmp")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lomp")
endif()

find_package(OpenMP REQUIRED)
if(OpenMP_FOUND)
    message(STATUS "Found OpenMP: ${OpenMP_CXX_FLAGS}")
else()
    message(FATAL_ERROR "OpenMP was not found, but it is required.")
endif()

################################################################################
# Find CGAL
################################################################################
find_package(CGAL QUIET)
if(NOT CGAL_FOUND)
    message(WARNING "This project requires CGAL but CGAL was not found.
Please install CGAL and set 'CGAL_DIR' to the directory containing CGALConfig.cmake or cgal-config.cmake.")
    return()
endif()

################################################################################
# Configure Project-Specific Paths
################################################################################
set(POLYGONIZATION_ROOT             ${CMAKE_CURRENT_LIST_DIR})
set(POLYGONIZATION_3RD_PARTY_ROOT   ${POLYGONIZATION_ROOT}/3rd_party)

# Use a plural variable name to indicate this is a list of include directories.
set(POLYGONIZATION_INCLUDE_DIRS      ${POLYGONIZATION_ROOT} ${CMAKE_CURRENT_BINARY_DIR})
set(POLYGONIZATION_rply_DIR         ${POLYGONIZATION_3RD_PARTY_ROOT}/rply)

################################################################################
# Set Up Output Directories
################################################################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)

################################################################################
# Add Subdirectories
################################################################################
add_subdirectory(3rd_party)
add_subdirectory(Polygonization)
