add_executable(
    tests
    ast/test_comments.cpp
    ast/test_constraints.cpp
    ast/test_declarations.cpp
    ast/test_expressions.cpp
    ast/test_newlines.cpp
    cli/test_argument_parser.cpp
    cli/test_config_reader.cpp
)

target_link_libraries(
    tests
    PRIVATE
        Catch2::Catch2WithMain
        ast
        builder
        cli
        emit
)

target_include_directories(
    tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/tests
        ${GENERATED_DIR}
)

# Copy test data and define macro for C++
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_compile_definitions(
    tests
    PRIVATE
        TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

include(CTest)
include(Catch)
catch_discover_tests(tests)
