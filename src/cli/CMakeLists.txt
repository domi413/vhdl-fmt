add_executable(vhdl_fmt main.cpp ArgumentParser.cpp ConfigReader.cpp)

target_link_libraries(
    vhdl_fmt
    PRIVATE
        vhdl_fmt_options
        vhdl_fmt_formatter
        vhdl_fmt_parser
        argparse::argparse
        yaml-cpp::yaml-cpp
    PUBLIC vhdl_fmt_common
)

target_include_directories(vhdl_fmt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
