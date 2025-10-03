add_library(
    vhdl_fmt_parser
    STATIC
    placeholder.cpp # NOTE: DELETE THIS LATER
    # Add your parser source files here
)

target_link_libraries(
    vhdl_fmt_parser
    PUBLIC vhdl_fmt_options vhdl_fmt_common antlr4_static
)

target_include_directories(vhdl_fmt_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
