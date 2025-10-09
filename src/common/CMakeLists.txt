file(GLOB_RECURSE BUILDER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(common STATIC ${BUILDER_SOURCES})

target_link_libraries(
    common
    PUBLIC yaml-cpp::yaml-cpp
)