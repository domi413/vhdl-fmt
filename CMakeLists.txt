cmake_minimum_required(VERSION 3.31)

# ==================================================================
# Project Definition
# ==================================================================

project(vhdl_fmt VERSION 0.1.0 DESCRIPTION "A VHDL parser" LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING
        "Choose the type of build."
        FORCE
    )
endif()

# ==================================================================
# Global Compiler Flags
# ==================================================================

add_library(vhdl_fmt_options INTERFACE)
target_compile_options(
    vhdl_fmt_options
    INTERFACE
        # Optimization
        -O3
        # Warnings
        -Wall
        -Wcast-align
        -Wconversion
        -Wdouble-promotion
        -Werror
        -Wextra
        -Wformat=2
        -Wmisleading-indentation
        -Wnon-virtual-dtor
        -Wnull-dereference
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpedantic
        -Wshadow
        -Wsign-conversion
        -Wunused
)

# ==================================================================
# Dependencies
# ==================================================================

find_package(antlr4-runtime REQUIRED)
find_package(argparse REQUIRED)
find_package(Catch2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)

# ==================================================================
# Subdirectory Delegation
# ==================================================================

add_subdirectory(src)

# enable_testing()
# add_subdirectory(tests)

# ==================================================================
# Build Info
# ==================================================================

message(STATUS "VHDL Parser:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build: ${CMAKE_BUILD_TYPE}")
message(
    STATUS
    "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
)
message(STATUS "  Standard: C++${CMAKE_CXX_STANDARD}")
