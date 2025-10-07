cmake_minimum_required(VERSION 3.30.0)
project(vhdl_fmt VERSION 0.1.0 DESCRIPTION "A VHDL formatter" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message(STATUS "Building ${PROJECT_NAME} ${PROJECT_VERSION}")

find_package(antlr4-runtime CONFIG REQUIRED)
find_package(argparse REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

# Generated ANTLR sources
file(GLOB ANTLR_GENERATED_SOURCES
    "${CMAKE_SOURCE_DIR}/build/generated/vhdl*.cpp"
)
set_source_files_properties(${ANTLR_GENERATED_SOURCES} PROPERTIES COMPILE_OPTIONS "-w")

add_library(vhdl_generated STATIC ${ANTLR_GENERATED_SOURCES})
target_include_directories(vhdl_generated PUBLIC
    ${CMAKE_SOURCE_DIR}/build/generated
)
target_link_libraries(vhdl_generated PUBLIC antlr4_static)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
