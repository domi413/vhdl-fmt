# Propagate includes to submodules
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Submodules
add_subdirectory(ast)
add_subdirectory(cli)
add_subdirectory(builder)
add_subdirectory(emit)

target_link_libraries(
    vhdl_formatter
    PRIVATE
        ast
        cli
        builder
        emit
        vhdl_generated
        argparse::argparse
        fmt::fmt
        spdlog::spdlog
)

target_compile_options(
    vhdl_formatter
    PRIVATE
        -O3
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
