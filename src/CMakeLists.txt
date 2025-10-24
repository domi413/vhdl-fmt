# Propagate includes to submodules
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Submodules
add_subdirectory(ast)
add_subdirectory(cli)
add_subdirectory(builder)
add_subdirectory(common)
add_subdirectory(emit)

# Main executable
add_executable(vhdl_formatter main.cpp)

target_link_libraries(
    vhdl_formatter
    PRIVATE
        ast
        common
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
        $<$<CONFIG:Release>:-O3>
        # -O3 -ffast-math # lossy floating-point optimizations
        # -flto=full # Apparently, this could improve performance by around 10%
        -Wall
        -Wcast-align
        -Wconversion
        -Wdeprecated
        -Wdouble-promotion
        -Werror
        -Wextra
        -Wfloat-conversion
        -Wfloat-equal
        -Wfor-loop-analysis
        -Wformat=2
        -Winvalid-utf8
        -Wmisleading-indentation
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpedantic
        -Wpoison-system-directories
        -Wshadow-all
        -Wsign-conversion
        -Wsuggest-destructor-override
        -Wsuggest-override
        -Wsuper-class-method-mismatch
        -Wunused
)
