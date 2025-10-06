add_executable(vhdl_fmt main.cpp ArgumentParser.cpp ConfigReader.cpp)

target_link_libraries(
    vhdl_fmt
    PRIVATE
        vhdl_fmt_options
        argparse::argparse
        yaml-cpp::yaml-cpp
    PUBLIC vhdl_fmt_common
)

target_include_directories(vhdl_fmt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
