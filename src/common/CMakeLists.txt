add_library(common INTERFACE)

target_link_libraries(
    common
    INTERFACE
        spdlog::spdlog
        fmt::fmt
)
