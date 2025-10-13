file(GLOB_RECURSE CLI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(common STATIC ${CLI_SOURCES})

target_link_libraries(
    common
    PUBLIC
        spdlog::spdlog
        fmt::fmt
)
