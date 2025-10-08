file(GLOB_RECURSE CLI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(cli STATIC ${CLI_SOURCES})

target_link_libraries(
    cli
    PUBLIC argparse::argparse yaml-cpp::yaml-cpp vhdl_generated
)
