add_library(
    cli
    STATIC
    argument_parser.cpp
    config_reader.cpp
)

target_include_directories(cli PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(
    cli
    PUBLIC
        common
    PRIVATE
        argparse::argparse
        yaml-cpp::yaml-cpp
)
