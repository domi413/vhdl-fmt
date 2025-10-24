#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "emit/debug_printer.hpp"

#include <string_view>
#include <vector>

namespace emit {

auto DebugPrinter::operator()(const ast::DesignFile &node) -> void
{
    emitNodeLike(node, "DesignFile", "");
    const IndentGuard _{ indent_ };
    visit(node.units);
}

auto DebugPrinter::operator()(const ast::Entity &node) -> void
{
    emitNodeLike(node, "Entity", node.name);
    const IndentGuard _{ indent_ };
    (*this)(node.generic_clause);
    (*this)(node.port_clause);

    if (!node.decls.empty()) {
        printLine("Declarations:");
        const IndentGuard _{ indent_ };
        visit(node.decls);
    }

    if (!node.stmts.empty()) {
        printLine("Statements:");
        const IndentGuard _{ indent_ };
        visit(node.stmts);
    }
}

auto DebugPrinter::operator()(const ast::Architecture &node) -> void
{
    emitNodeLike(node, "Architecture", node.name + " of " + node.entity_name);
    const IndentGuard _{ indent_ };
    visit(node.decls);
    visit(node.stmts);
}

auto DebugPrinter::operator()(const ast::GenericClause &node) -> void
{
    emitNodeLike(node, "GenericClause", "");
    const IndentGuard _{ indent_ };
    visit(node.generics);
}

auto DebugPrinter::operator()(const ast::PortClause &node) -> void
{
    emitNodeLike(node, "PortClause", "");
    const IndentGuard _{ indent_ };
    visit(node.ports);
}

} // namespace emit
