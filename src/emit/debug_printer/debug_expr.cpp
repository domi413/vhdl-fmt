#include "ast/nodes/expressions.hpp"
#include "emit/debug_printer.hpp"

namespace emit {

auto DebugPrinter::visit(const ast::TokenExpr &node) -> void
{
    emitNodeLike(node, "TokenExpr", node.text);
}

auto DebugPrinter::visit(const ast::GroupExpr &node) -> void
{
    emitNodeLike(node, "GroupExpr", "");
    const IndentGuard _{ indent_ };
    dispatchAll(node.children);
}

auto DebugPrinter::visit(const ast::UnaryExpr &node) -> void
{
    emitNodeLike(node, "UnaryExpr", node.op);
    const IndentGuard _{ indent_ };

    if (node.value) {
        printLine("value:");
        const IndentGuard _{ indent_ };
        dispatch(node.value);
    }
}

auto DebugPrinter::visit(const ast::BinaryExpr &node) -> void
{
    emitNodeLike(node, "BinaryExpr", node.op);
    const IndentGuard _{ indent_ };

    if (node.left) {
        printLine("left:");
        const IndentGuard _{ indent_ };
        dispatch(node.left);
    }

    if (node.right) {
        printLine("right:");
        const IndentGuard _{ indent_ };
        dispatch(node.right);
    }
}

auto DebugPrinter::visit(const ast::ParenExpr &node) -> void
{
    emitNodeLike(node, "ParenExpr", "");
    const IndentGuard _{ indent_ };

    if (node.inner) {
        printLine("inner:");
        const IndentGuard _{ indent_ };
        dispatch(node.inner);
    }
}

} // namespace emit
