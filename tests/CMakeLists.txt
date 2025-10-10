file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(tests ${TEST_SOURCES})

target_link_libraries(
    tests
    PRIVATE
        Catch2::Catch2WithMain
        ast
        builder
        emit
        vhdl_generated
)

target_include_directories(
    tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/build/generated
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
