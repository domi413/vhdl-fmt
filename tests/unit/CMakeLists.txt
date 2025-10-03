add_executable(
    unit_tests
    placeholder_test.cpp
    # Add your unit test source files here
)

target_link_libraries(
    unit_tests
    PRIVATE
        Catch2::Catch2WithMain
        vhdl_fmt_parser
        vhdl_fmt_common
        vhdl_fmt_formatter
)
