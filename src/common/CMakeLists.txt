configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.hpp
    @ONLY
)

add_library(
    vhdl_fmt_common
    STATIC
    logger.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/version.hpp
)

target_link_libraries(vhdl_fmt_common PUBLIC vhdl_fmt_options spdlog::spdlog)

target_include_directories(
    vhdl_fmt_common
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)
