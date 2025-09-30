add_library(
    vhdl_fmt_formatter
    STATIC
    # Add your formatter source files here
)

target_link_libraries(
    vhdl_fmt_formatter
    PUBLIC vhdl_fmt_options vhdl_fmt_common vhdl_fmt_parser
)

target_include_directories(
    vhdl_fmt_formatter
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
