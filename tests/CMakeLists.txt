add_executable(
    tests
    ast/nodes/design_units/test_comments.cpp
    ast/nodes/design_units/test_newlines.cpp
    ast/nodes/design_units/test_entity.cpp
    ast/nodes/design_units/test_architecture.cpp
    ast/nodes/design_units/test_package.cpp
    ast/nodes/design_units/test_package_body.cpp
    ast/nodes/design_units/test_configuration.cpp
    ast/nodes/design_units/test_context.cpp
    ast/nodes/design_units/test_package_instantiation.cpp
    ast/nodes/design_units/test_interface_package.cpp
    ast/nodes/design_units/test_entity_instantiation.cpp
    ast/nodes/declarations/test_constraints.cpp
    ast/nodes/declarations/test_declarations.cpp
    ast/nodes/expressions/test_expressions.cpp
    ast/nodes/statements/test_statements.cpp
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
