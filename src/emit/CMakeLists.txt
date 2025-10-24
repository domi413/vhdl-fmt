add_library(
    emit
    STATIC
        debug_printer/debug_core.cpp
        debug_printer/debug_decl.cpp
        debug_printer/debug_expr.cpp
        debug_printer/debug_stmt.cpp
        debug_printer/debug_unit.cpp
)

target_include_directories(
    emit
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    emit
    PUBLIC
        ast
)
