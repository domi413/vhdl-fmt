add_executable(
    emit_tests
    pretty_printer/test_doc.cpp
)

target_link_libraries(
    emit_tests
    PRIVATE
        Catch2::Catch2WithMain
        emit
)

target_include_directories(
    emit_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/tests
        ${GENERATED_DIR}
)

# Macro for test data directory
target_compile_definitions(
    emit_tests
    PRIVATE
        TEST_DATA_DIR="${CMAKE_BINARY_DIR}/tests/data"
)

catch_discover_tests(emit_tests)
