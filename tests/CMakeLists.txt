find_package(antlr4-runtime CONFIG REQUIRED)
find_package(Catch2 REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(
    tests
    test_ports.cpp
    ../src/builder/visitor.cpp
    ../src/builder/translator.cpp
    ../build/generated/vhdlLexer.cpp
    ../build/generated/vhdlParser.cpp
    ../build/generated/vhdlBaseVisitor.cpp
    ../build/generated/vhdlVisitor.cpp
)

target_link_libraries(
    tests
    PRIVATE Catch2::Catch2WithMain fmt::fmt antlr4_static
)

target_include_directories(
    tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/build/generated
)

# Copy test data and define path
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(tests PRIVATE
    TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

include(Catch)
catch_discover_tests(tests)
