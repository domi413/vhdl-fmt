file(GLOB_RECURSE BUILDER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(builder STATIC ${BUILDER_SOURCES})

target_link_libraries(builder PUBLIC ast vhdl_generated)
