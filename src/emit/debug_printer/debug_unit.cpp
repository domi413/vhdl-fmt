#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "emit/debug_printer.hpp"

#include <string_view>
#include <vector>

namespace emit {

auto DebugPrinter::visit(const ast::DesignFile &node) -> void
{
    emitNodeLike(node, "DesignFile", "");
    const IndentGuard _{ indent_ };
    walk(node);
}

auto DebugPrinter::visit(const ast::Entity &node) -> void
{
    emitNodeLike(node, "Entity", node.name);
    const IndentGuard _{ indent_ };

    if (node.generic_clause) {
        node.generic_clause->accept(*this);
    }
    if (node.port_clause) {
        node.port_clause->accept(*this);
    }

    if (!node.decls.empty()) {
        printLine("Declarations:");
        const IndentGuard _{ indent_ };
        for (const auto &d : node.decls) {
            if (d) {
                dispatch(d);
            }
        }
    }

    if (!node.stmts.empty()) {
        printLine("Statements:");
        const IndentGuard _{ indent_ };
        for (const auto &s : node.stmts) {
            if (s) {
                dispatch(s);
            }
        }
    }
}

auto DebugPrinter::visit(const ast::Architecture &node) -> void
{
    emitNodeLike(node, "Architecture", node.name + " of " + node.entity_name);
    const IndentGuard _{ indent_ };
    walk(node);
}

auto DebugPrinter::visit(const ast::GenericClause &node) -> void
{
    emitNodeLike(node, "GenericClause", "");
    const IndentGuard _{ indent_ };
    walk(node);
}

auto DebugPrinter::visit(const ast::PortClause &node) -> void
{
    emitNodeLike(node, "PortClause", "");
    const IndentGuard _{ indent_ };
    walk(node);
}

} // namespace emit
