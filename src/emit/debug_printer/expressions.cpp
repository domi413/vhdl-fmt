#include "ast/nodes/expressions.hpp"

#include "emit/debug_printer.hpp"

namespace emit {

void DebugPrinter::visit(const ast::TokenExpr &node)
{
    emitNodeLike(node, "TokenExpr", node.text);
}

void DebugPrinter::visit(const ast::GroupExpr &node)
{
    emitNodeLike(node, "GroupExpr", "");
    const IndentGuard _{ indent_ };
    dispatchAll(node.children);
}

void DebugPrinter::visit(const ast::UnaryExpr &node)
{
    emitNodeLike(node, "UnaryExpr", node.op);
    const IndentGuard _{ indent_ };

    if (node.value) {
        printLine("value:");
        const IndentGuard _{ indent_ };
        node.value->accept(*this);
    }
}

void DebugPrinter::visit(const ast::BinaryExpr &node)
{
    emitNodeLike(node, "BinaryExpr", node.op);
    const IndentGuard _{ indent_ };

    if (node.left) {
        printLine("left:");
        const IndentGuard _{ indent_ };
        node.left->accept(*this);
    }

    if (node.right) {
        printLine("right:");
        const IndentGuard _{ indent_ };
        node.right->accept(*this);
    }
}

void DebugPrinter::visit(const ast::ParenExpr &node)
{
    emitNodeLike(node, "ParenExpr", "");
    const IndentGuard _{ indent_ };

    if (node.inner) {
        printLine("inner:");
        const IndentGuard _{ indent_ };
        node.inner->accept(*this);
    }
}

} // namespace emit
