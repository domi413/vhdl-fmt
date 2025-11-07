cmake_minimum_required(VERSION 3.30)
project(vhdl_fmt VERSION 0.1.0 DESCRIPTION "A VHDL formatter" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_VERIFY_INTERFACE_HEADER_SETS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "  Version:  ${PROJECT_VERSION}")
message(STATUS "  Build:    ${CMAKE_BUILD_TYPE}")
message(
    STATUS
    "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
)
message(STATUS "  Standard: C++${CMAKE_CXX_STANDARD}")

# Enable ccache for faster rebuilds if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "  ccache:   ${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

find_package(antlr4-runtime CONFIG REQUIRED)
find_package(argparse REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

# --------------------------------------------------------------------
# ANTLR Generation
# --------------------------------------------------------------------
set(GRAMMAR_DIR ${CMAKE_SOURCE_DIR}/grammars)
set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)

set(GRAMMAR_FILES
    ${GRAMMAR_DIR}/vhdlLexer.g4
    ${GRAMMAR_DIR}/vhdlParser.g4
)

find_program(ANTLR4_EXECUTABLE antlr4 REQUIRED)

# Tell CMake how to generate these sources
add_custom_command(
    OUTPUT
        ${GENERATED_DIR}/vhdlLexer.cpp
        ${GENERATED_DIR}/vhdlLexer.h
        ${GENERATED_DIR}/vhdlParser.cpp
        ${GENERATED_DIR}/vhdlParser.h
        ${GENERATED_DIR}/vhdlLexer.tokens
        ${GENERATED_DIR}/vhdlParser.tokens
    COMMAND
        ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
    COMMAND
        ${ANTLR4_EXECUTABLE} -Dlanguage=Cpp ${GRAMMAR_FILES} -lib ${GRAMMAR_DIR}
        -o ${GENERATED_DIR} -no-listener
    DEPENDS
        ${GRAMMAR_FILES}
    COMMENT "Generating VHDL lexer and parser using ANTLR4"
    VERBATIM
)

# Create the library from the generated sources
add_library(
    vhdl_generated
    STATIC
    ${GENERATED_DIR}/vhdlLexer.cpp
    ${GENERATED_DIR}/vhdlParser.cpp
)

# Mark generated sources explicitly
set_source_files_properties(
    ${GENERATED_DIR}/vhdlLexer.cpp
    ${GENERATED_DIR}/vhdlParser.cpp
    PROPERTIES
        GENERATED
            TRUE
)

# Mark generated headers as SYSTEM to suppress warnings in generated code
target_include_directories(vhdl_generated SYSTEM PUBLIC ${GENERATED_DIR})
target_link_libraries(vhdl_generated PUBLIC antlr4_static)

# --------------------------------------------------------------------
# Version Header Generation
# --------------------------------------------------------------------
set(VERSION_IN_FILE "${CMAKE_SOURCE_DIR}/src/common/version.hpp.in")
set(VERSION_OUT_FILE "${CMAKE_BINARY_DIR}/generated/version.hpp")

# Ensure the output directory exists
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")

configure_file(${VERSION_IN_FILE} ${VERSION_OUT_FILE} @ONLY)

# --------------------------------------------------------------------
# Project subdirs
# --------------------------------------------------------------------
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
