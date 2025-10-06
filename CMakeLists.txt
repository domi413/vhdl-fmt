cmake_minimum_required(VERSION 4.0.0)

# ==================================================================
# Project Definition
# ==================================================================

project(vhdl_fmt VERSION 0.1.0 DESCRIPTION "A VHDL formatter" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

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

include(CTest)
enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
