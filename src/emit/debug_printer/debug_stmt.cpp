#include "ast/nodes/statements.hpp"
#include "emit/debug_printer.hpp"

// NOLINTBEGIN(misc-no-recursion)

namespace emit {

auto DebugPrinter::operator()(const ast::ConcurrentAssign &node) -> void
{
    emitNodeLike(node, "ConcurrentAssign", "");
    const IndentGuard _{ indent_ };
    visit(node.target);
    visit(node.value);
}

auto DebugPrinter::operator()(const ast::Process &node) -> void
{
    emitNodeLike(node, "Process", "");
    const IndentGuard _{ indent_ };
    visit(node.body);
    // visit(node.sensitivity_list);
}

} // namespace emit

// NOLINTEND(misc-no-recursion)
