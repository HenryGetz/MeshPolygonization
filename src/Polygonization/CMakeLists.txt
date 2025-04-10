cmake_minimum_required(VERSION 3.10)
project(MeshPolygonization)

# Tell CMake to prefer pthreads; fixes issues with Homebrew’s LLVM on macOS.
set(THREADS_PREFER_PTHREAD_FLAG ON)

# --- Configure OpenMP for macOS using AppleClang and Homebrew's libomp ---
if(APPLE)
    message(STATUS "Configuring OpenMP for macOS with AppleClang using Homebrew libomp")
    # Append OpenMP flags to the compiler and linker flags.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xpreprocessor -fopenmp")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lomp")
endif()

# Find OpenMP after setting the appropriate flags.
find_package(OpenMP REQUIRED)
if(NOT OpenMP_FOUND)
    message(FATAL_ERROR "OpenMP not found. Please ensure libomp is installed and your compiler is correctly configured.")
endif()
message(STATUS "Found OpenMP: ${OpenMP_CXX_FLAGS}")

# List header and source files.
set(MeshPolygonization_HEADERS
    CandidateFace.h
    CGALTypes.h
    Intersection.h
    Optimization.h
    Orientation.h
    Planarity.h
    PlanarSegmentation.h
    Segment.h
    Simplification.h
    StructureGraph.h
    Utils.h
    solver/linear_program.h
    solver/linear_program_solver.h
)

set(MeshPolygonization_SOURCES
    main.cpp
    Planarity.cpp
    PlanarSegmentation.cpp
    Simplification.cpp
    StructureGraph.cpp
    solver/linear_program.cpp
    solver/linear_program_solver.cpp
    solver/linear_program_solver_GUROBI.cpp
)

# Create the executable.
add_executable(MeshPolygonization ${MeshPolygonization_SOURCES} ${MeshPolygonization_HEADERS})

# Enforce C++11 for this target (if not already globally set)
target_compile_features(MeshPolygonization PRIVATE cxx_std_11)

# ------------------------------------------------------------------------------
# Include external directories and link third-party libraries.
# ------------------------------------------------------------------------------
target_include_directories(MeshPolygonization PRIVATE ${POLYGONIZATION_rply_DIR})
target_link_libraries(MeshPolygonization PRIVATE
    3rd_rply
    OpenMP::OpenMP_CXX
)

# ------------------------------------------------------------------------------
# Define the resources directory.
# ------------------------------------------------------------------------------
target_compile_definitions(MeshPolygonization PRIVATE
    "POLYGONIZATION_ROOT_DIR=\"${POLYGONIZATION_ROOT}\""
)

# ------------------------------------------------------------------------------
# GUROBI Setup
# ------------------------------------------------------------------------------
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindGUROBI.cmake)
if(GUROBI_FOUND)
    message(STATUS "Gurobi include dir: ${GUROBI_INCLUDE_DIRS}")
    message(STATUS "Gurobi libraries: ${GUROBI_LIBRARIES}")

    target_compile_definitions(MeshPolygonization PUBLIC HAS_GUROBI)
    target_include_directories(MeshPolygonization PRIVATE ${GUROBI_INCLUDE_DIRS})
    target_link_libraries(MeshPolygonization PRIVATE ${GUROBI_LIBRARIES})

    # Find and link the Gurobi C++ API library.
    find_library(GUROBI_CXX_LIBRARY
        NAMES gurobi_c++
        PATHS ${GUROBI_LIBRARY_DIR}    # This should be set inside your FindGUROBI.cmake.
    )
    if(GUROBI_CXX_LIBRARY)
        target_link_libraries(MeshPolygonization PRIVATE ${GUROBI_CXX_LIBRARY})
    else()
        message(FATAL_ERROR "Could not find Gurobi C++ library (gurobi_c++)")
    endif()
endif()

# ------------------------------------------------------------------------------
# CGAL Setup
# ------------------------------------------------------------------------------
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/UseCGAL.cmake)
