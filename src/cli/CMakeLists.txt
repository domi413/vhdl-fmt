file(GLOB_RECURSE BUILDER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(cli STATIC ${BUILDER_SOURCES})

target_link_libraries(cli PUBLIC common vhdl_generated)
