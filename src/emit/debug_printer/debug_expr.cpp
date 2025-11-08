#include "ast/nodes/expressions.hpp"
#include "emit/debug_printer.hpp"

namespace emit {

auto DebugPrinter::operator()(const ast::TokenExpr &node) -> void
{
    emitNodeLike(node, "TokenExpr", node.text);
}

auto DebugPrinter::operator()(const ast::GroupExpr &node) -> void
{
    emitNodeLike(node, "GroupExpr", "");
    const IndentGuard _{ indent_ };
    visit(node.children);
}

auto DebugPrinter::operator()(const ast::UnaryExpr &node) -> void
{
    emitNodeLike(node, "UnaryExpr", node.op);
    const IndentGuard _{ indent_ };

    if (node.value) {
        printLine("value:");
        const IndentGuard _{ indent_ };
        visit(node.value);
    }
}

auto DebugPrinter::operator()(const ast::BinaryExpr &node) -> void
{
    emitNodeLike(node, "BinaryExpr", node.op);
    const IndentGuard _{ indent_ };

    if (node.left) {
        printLine("left:");
        const IndentGuard _{ indent_ };
        visit(node.left);
    }

    if (node.right) {
        printLine("right:");
        const IndentGuard _{ indent_ };
        visit(node.right);
    }
}

auto DebugPrinter::operator()(const ast::ParenExpr &node) -> void
{
    emitNodeLike(node, "ParenExpr", "");
    const IndentGuard _{ indent_ };

    if (node.inner) {
        printLine("inner:");
        const IndentGuard _{ indent_ };
        visit(node.inner);
    }
}

auto DebugPrinter::operator()(const ast::CallExpr &node) -> void
{
    emitNodeLike(node, "CallExpr", "");
    const IndentGuard _{ indent_ };

    if (node.callee) {
        printLine("callee:");
        const IndentGuard _{ indent_ };
        visit(node.callee);
    }

    if (node.args) {
        printLine("args:");
        const IndentGuard _{ indent_ };
        visit(node.args);
    }
}

auto DebugPrinter::operator()(const ast::IndexConstraint &node) -> void
{
    emitNodeLike(node, "IndexConstraint", "");
    const IndentGuard _{ indent_ };
    visit(node.ranges);
}

auto DebugPrinter::operator()(const ast::RangeConstraint &node) -> void
{
    emitNodeLike(node, "RangeConstraint", "");
    const IndentGuard _{ indent_ };
    visit(node.range);
}

} // namespace emit
