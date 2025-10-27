add_executable(
    tests
    test_argument_parser.cpp
    test_comments.cpp
    test_config_reader.cpp
    test_constraints.cpp
    test_declarations.cpp
    test_expressions.cpp
    test_newlines.cpp
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
