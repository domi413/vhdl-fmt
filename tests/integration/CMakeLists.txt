add_executable(
    integration_tests
    placeholder_test.cpp
    # Add your integration test source files here
)

target_link_libraries(
    integration_tests
    PRIVATE
        Catch2::Catch2WithMain
        vhdl_fmt_formatter
        vhdl_fmt_parser
        vhdl_fmt_common
)
