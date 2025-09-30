cmake_minimum_required(VERSION 3.31)

# ==================================================================
# Project Definition
# ==================================================================

project(vhdl_parser VERSION 0.1.0 DESCRIPTION "A VHDL parser" LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ==================================================================
# Dependencies
# ==================================================================

find_package(spdlog REQUIRED)
find_package(Catch2 REQUIRED)
find_package(argparse REQUIRED)
find_package(antlr4-runtime REQUIRED)

# ==================================================================
# Main Executable
# ==================================================================

target_link_libraries(
    vhdl_parser
    PRIVATE spdlog::spdlog argparse::argparse antlr4_static
)

# ==================================================================
# Compiler Flags
# ==================================================================

target_compile_options(
    vhdl_parser
    PRIVATE
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
        # -Wlifetime # Not available yet
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
