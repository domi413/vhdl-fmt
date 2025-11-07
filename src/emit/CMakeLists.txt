add_library(
    emit
    STATIC
    debug_printer/debug_core.cpp
    debug_printer/debug_decl.cpp
    debug_printer/debug_expr.cpp
    debug_printer/debug_stmt.cpp
    debug_printer/debug_unit.cpp
    pretty_printer/doc.cpp
    pretty_printer/doc_impl.cpp
    pretty_printer/renderer.cpp
    pretty_printer/table.cpp
    pretty_printer/nodes/design_file.cpp
    pretty_printer/nodes/design_units.cpp
    pretty_printer/nodes/clauses.cpp
    pretty_printer/nodes/declarations.cpp
    pretty_printer/nodes/expressions.cpp
    pretty_printer/nodes/statements.cpp
)

target_include_directories(emit PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(emit PUBLIC ast)
