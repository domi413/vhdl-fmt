add_library(common INTERFACE)

file(GLOB_RECURSE COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(
    common
    INTERFACE
        FILE_SET HEADERS
        FILES ${COMMON_HEADERS}
)

target_link_libraries(
    common
    INTERFACE
        spdlog::spdlog
        fmt::fmt
)
