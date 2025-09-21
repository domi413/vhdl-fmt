# TODO(domi413): THIS FILE IS JUST A TEMPLATE AND NOT READY YET
cmake_minimum_required(VERSION 4.1)

# ==================================================================
# Project Definition
# ==================================================================

set(CMAKE_CXX_COMPILER "clang++")
project(
    vhdl-fmt
    VERSION 0.1.0
    DESCRIPTION "VHDL Formatter"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ==================================================================
# Version Information
# ==================================================================

find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    execute_process(
        COMMAND ${GIT_EXECUTABLE} diff --quiet
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_DIRTY
        ERROR_QUIET
    )
    if(NOT GIT_DIRTY EQUAL 0)
        set(GIT_COMMIT "${GIT_COMMIT}-dirty")
    endif()
else()
    set(GIT_COMMIT "unknown")
endif()

configure_file(
    typetrace/common/include/version.hpp.in
    ${CMAKE_BINARY_DIR}/generated/version.hpp
    @ONLY
)

# ==================================================================
# Dependencies
# ==================================================================

find_package(PkgConfig REQUIRED)
find_package(spdlog CONFIG REQUIRED)


# ==================================================================
# Subdirectories
# ==================================================================

add_subdirectory(tests)

# ==================================================================
# Installation & Build Info
# ==================================================================

install(TARGETS typetrace_backend RUNTIME DESTINATION bin)

message(STATUS "${PROJECT_NAME}")
message(STATUS "  Version: ${PROJECT_VERSION} (${GIT_COMMIT})")
message(STATUS "  Build: ${CMAKE_BUILD_TYPE}")
message(
    STATUS
    "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
)
message(STATUS "  Standard: C++${CMAKE_CXX_STANDARD}")
