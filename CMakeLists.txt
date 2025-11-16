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

# --------------------------------------------------------------------
# Coverage Option
# --------------------------------------------------------------------
option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "  Coverage: ENABLED")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Use LLVM source-based coverage for Clang
        add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
        add_link_options(-fprofile-instr-generate -fcoverage-mapping)
        message(STATUS "  Using LLVM source-based coverage")
    else()
        # Use gcov-compatible coverage for GCC
        add_compile_options(--coverage)
        add_link_options(--coverage)
        message(STATUS "  Using gcov-compatible coverage")
    endif()
endif()

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

add_library(
    vhdl_generated
    STATIC
    ${GENERATED_DIR}/vhdlLexer.cpp
    ${GENERATED_DIR}/vhdlParser.cpp
)

set_source_files_properties(
    ${GENERATED_DIR}/vhdlLexer.cpp
    ${GENERATED_DIR}/vhdlParser.cpp
    PROPERTIES
        GENERATED TRUE
)

target_include_directories(vhdl_generated SYSTEM PUBLIC ${GENERATED_DIR})
target_link_libraries(vhdl_generated PUBLIC antlr4_static)

# --------------------------------------------------------------------
# Version Header Generation
# --------------------------------------------------------------------
set(VERSION_IN_FILE "${CMAKE_SOURCE_DIR}/src/common/version.hpp.in")
set(VERSION_OUT_FILE "${CMAKE_BINARY_DIR}/generated/version.hpp")

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")
configure_file(${VERSION_IN_FILE} ${VERSION_OUT_FILE} @ONLY)

# --------------------------------------------------------------------
# Project subdirs
# --------------------------------------------------------------------
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

# --------------------------------------------------------------------
# Coverage Target
# --------------------------------------------------------------------
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Use LLVM coverage tools for Clang
        find_program(LLVM_PROFDATA_EXECUTABLE llvm-profdata REQUIRED)
        find_program(LLVM_COV_EXECUTABLE llvm-cov REQUIRED)

        add_custom_target(
            coverage
            COMMAND ${CMAKE_COMMAND} -E echo "Running tests to generate coverage data..."
            COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/coverage/vhdl_fmt-%p.profraw
                    ${CMAKE_CTEST_COMMAND} --preset conan-debug --output-on-failure || true
            COMMAND ${CMAKE_COMMAND} -E echo "Merging coverage data..."
            COMMAND ${LLVM_PROFDATA_EXECUTABLE} merge -sparse ${CMAKE_BINARY_DIR}/coverage/vhdl_fmt-*.profraw
                    -o ${CMAKE_BINARY_DIR}/coverage/vhdl_fmt.profdata
            COMMAND ${CMAKE_COMMAND} -E echo "Generating coverage report..."
            COMMAND ${LLVM_COV_EXECUTABLE} show ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vhdl_formatter
                    -instr-profile=${CMAKE_BINARY_DIR}/coverage/vhdl_fmt.profdata
                    -format=html -output-dir=${CMAKE_BINARY_DIR}/coverage/html
                    -ignore-filename-regex='${CMAKE_BINARY_DIR}/generated/.*'
                    -ignore-filename-regex='${CMAKE_BINARY_DIR}/.*_autogen/.*'
                    ${CMAKE_SOURCE_DIR}/src/
            COMMAND ${CMAKE_COMMAND} -E echo "Coverage HTML report: ${CMAKE_BINARY_DIR}/coverage/html/index.html"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating coverage report with llvm-cov"
        )

        add_custom_target(
            coverage-report
            COMMAND ${CMAKE_COMMAND} -E echo "Running tests to generate coverage data..."
            COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/coverage/vhdl_fmt-%p.profraw
                    ${CMAKE_CTEST_COMMAND} --preset conan-debug --output-on-failure || true
            COMMAND ${CMAKE_COMMAND} -E echo "Merging coverage data..."
            COMMAND ${LLVM_PROFDATA_EXECUTABLE} merge -sparse ${CMAKE_BINARY_DIR}/coverage/vhdl_fmt-*.profraw
                    -o ${CMAKE_BINARY_DIR}/coverage/vhdl_fmt.profdata
            COMMAND ${CMAKE_COMMAND} -E echo "Coverage Summary:"
            COMMAND ${LLVM_COV_EXECUTABLE} report ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vhdl_formatter
                    -instr-profile=${CMAKE_BINARY_DIR}/coverage/vhdl_fmt.profdata
                    -ignore-filename-regex='${CMAKE_BINARY_DIR}/generated/.*'
                    ${CMAKE_SOURCE_DIR}/src/
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Printing coverage summary"
        )

        # Clean coverage data
        add_custom_target(
            coverage-clean
            COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/coverage
            COMMAND ${CMAKE_COMMAND} -E echo "Removing .profraw and .profdata files..."
            COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/**/*.profraw ${CMAKE_BINARY_DIR}/**/*.profdata
            COMMENT "Cleaning coverage data"
        )
    else()
        # Use gcovr for GCC
        find_program(GCOVR_EXECUTABLE gcovr REQUIRED)

        add_custom_target(
            coverage
            COMMAND ${CMAKE_COMMAND} -E echo "Running tests to generate coverage data..."
            COMMAND ${CMAKE_CTEST_COMMAND} --preset conan-debug --output-on-failure || true
            COMMAND ${CMAKE_COMMAND} -E echo "Generating coverage report..."
            COMMAND ${GCOVR_EXECUTABLE}
                    --root ${CMAKE_SOURCE_DIR}
                    --filter '${CMAKE_SOURCE_DIR}/src/.*'
                    --exclude '${CMAKE_BINARY_DIR}/.*'
                    --exclude '.*/generated/.*'
                    --exclude '.*/tests/.*'
                    --exclude '.*/build/.*'
                    --html-details ${CMAKE_BINARY_DIR}/coverage/coverage.html
                    --xml ${CMAKE_BINARY_DIR}/coverage/coverage.xml
                    --txt ${CMAKE_BINARY_DIR}/coverage/coverage.txt
                    --print-summary
            COMMAND ${CMAKE_COMMAND} -E echo "Coverage HTML report: ${CMAKE_BINARY_DIR}/coverage/coverage.html"
            COMMAND ${CMAKE_COMMAND} -E echo "Coverage summary: ${CMAKE_BINARY_DIR}/coverage/coverage.txt"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating coverage report with gcovr"
        )

        add_custom_target(
            coverage-report
            COMMAND ${CMAKE_COMMAND} -E echo "Coverage Summary:"
            COMMAND ${GCOVR_EXECUTABLE}
                    --root ${CMAKE_SOURCE_DIR}
                    --filter '${CMAKE_SOURCE_DIR}/src/.*'
                    --exclude '${CMAKE_BINARY_DIR}/.*'
                    --exclude '.*/generated/.*'
                    --exclude '.*/tests/.*'
                    --exclude '.*/build/.*'
                    --print-summary
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Printing coverage summary"
        )

        # Clean coverage data
        add_custom_target(
            coverage-clean
            COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/coverage
            COMMAND ${CMAKE_COMMAND} -E echo "Removing .gcda files..."
            COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/**/*.gcda
            COMMENT "Cleaning coverage data"
        )
    endif()
endif()
