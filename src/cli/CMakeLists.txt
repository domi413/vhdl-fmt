file(GLOB_RECURSE CLI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(cli STATIC ${CLI_SOURCES})

add_executable(vhdl_formatter main.cpp)

target_link_libraries(cli PRIVATE yaml-cpp::yaml-cpp vhdl_generated)
