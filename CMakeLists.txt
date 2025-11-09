cmake_minimum_required(VERSION 3.10)

project(GroggScript)
# Add this to the top or after project(...)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif ()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Recursively collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(GroggScript ${SOURCES})

target_include_directories(GroggScript PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/include"
)
