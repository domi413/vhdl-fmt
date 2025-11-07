add_executable(
    cli_tests
    test_argument_parser.cpp
    test_config_reader.cpp
)

target_link_libraries(
    cli_tests
    PRIVATE
        Catch2::Catch2WithMain
        cli
)

target_include_directories(
    cli_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/tests
        ${GENERATED_DIR}
)

# Macro for test data directory
target_compile_definitions(
    cli_tests
    PRIVATE
        TEST_DATA_DIR="${CMAKE_BINARY_DIR}/tests/data"
)

catch_discover_tests(cli_tests)
