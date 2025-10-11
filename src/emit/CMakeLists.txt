file(GLOB_RECURSE EMIT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(emit STATIC ${EMIT_SOURCES})

target_link_libraries(
    emit
    PUBLIC
        ast
        vhdl_generated
)
