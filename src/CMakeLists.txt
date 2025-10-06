find_package(antlr4-runtime CONFIG REQUIRED)
find_package(argparse REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog REQUIRED)

add_executable(vhdl_formatter
    main.cpp
    builder/visitor.cpp
    builder/translator.cpp
    ../build/generated/vhdlLexer.cpp
    ../build/generated/vhdlParser.cpp
    ../build/generated/vhdlBaseVisitor.cpp
    ../build/generated/vhdlVisitor.cpp
)

target_link_libraries(vhdl_formatter PRIVATE 
    antlr4_static
    argparse::argparse
    fmt::fmt
    spdlog::spdlog
)

target_include_directories(vhdl_formatter
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/generated
)

target_compile_options(vhdl_formatter PRIVATE
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

file(GLOB ANTLR_GENERATED_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../build/generated/vhdl*.cpp"
)

set_source_files_properties(
    ${ANTLR_GENERATED_SOURCES}
    PROPERTIES COMPILE_OPTIONS "-w"
)