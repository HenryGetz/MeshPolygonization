# ------------------------------------------------------------------------------
# This file sets up Gurobi for CMake.
# Once done this will define:
#
#   GUROBI_FOUND        - system has GUROBI
#   GUROBI_INCLUDE_DIRS - the Gurobi include directories
#   GUROBI_LIBRARIES    - Link these to use Gurobi
# ------------------------------------------------------------------------------

# Is it already configured?
if (NOT GUROBI_FOUND)

    # Hardcoded search paths
    set(SEARCH_PATHS_FOR_HEADERS
            "$ENV{GUROBI_HOME}/include"
            "/Library/gurobi951/macos_universal2/include"
            "/Library/gurobi950/mac64/include"
            "/Library/gurobi950/macos_universal2/include"
            "C:\\dev\\gurobi950\\win64\\include"
            "/Library/gurobi901/mac64/include"
            "/home/liangliang/dev/gurobi901/include"
            "C:\\dev\\gurobi901\\win64\\include"
            )

    set(SEARCH_PATHS_FOR_LIBRARIES
            "$ENV{GUROBI_HOME}/lib"
            "/Library/gurobi951/macos_universal2/lib"
            "/Library/gurobi950/mac64/lib"
            "/Library/gurobi950/macos_universal2/lib"
            "C:\\dev\\gurobi950\\win64\\lib"
            "/Library/gurobi901/mac64/lib"
            "/home/liangliang/dev/gurobi901/lib"
            "C:\\dev\\gurobi901\\win64\\lib"
            )

    # Search for the headers and libraries
    find_path(GUROBI_INCLUDE_DIR gurobi_c++.h
            PATHS ${SEARCH_PATHS_FOR_HEADERS}
            )

    find_library(GUROBI_CXX_LIBRARY
            NAMES gurobi_c++
            PATHS ${SEARCH_PATHS_FOR_LIBRARIES}
            NO_DEFAULT_PATH
            )

    find_library(GUROBI_C_LIBRARY
            NAMES gurobi120 gurobi
            PATHS ${SEARCH_PATHS_FOR_LIBRARIES}
            NO_DEFAULT_PATH
            )

    # Set up the include directories and the libraries (without debug/optimized keywords)
    set(GUROBI_INCLUDE_DIRS ${GUROBI_INCLUDE_DIR})
    set(GUROBI_LIBRARIES ${GUROBI_CXX_LIBRARY} ${GUROBI_C_LIBRARY})
endif ()

# Check that Gurobi was successfully found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GUROBI DEFAULT_MSG GUROBI_INCLUDE_DIRS)
find_package_handle_standard_args(GUROBI DEFAULT_MSG GUROBI_LIBRARIES)

# Hide these variables from CMake GUI options.
mark_as_advanced(
        GUROBI_INCLUDE_DIRS
        GUROBI_INCLUDE_DIR
        GUROBI_LIBRARIES
        GUROBI_CXX_LIBRARY_DEBUG
        GUROBI_CXX_LIBRARY_RELEASE
        GUROBI_C_LIBRARY
)
