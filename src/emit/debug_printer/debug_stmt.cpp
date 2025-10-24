#include "ast/nodes/statements.hpp"
#include "emit/debug_printer.hpp"

#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>

// NOLINTBEGIN(misc-no-recursion)

namespace emit {

// Concurrent Statements

auto DebugPrinter::operator()(const ast::ConcurrentAssign &node) -> void
{
    emitNodeLike(node, "ConcurrentAssign", "<=");
    const IndentGuard _{ indent_ };
    printLine("target:");
    {
        const IndentGuard _{ indent_ };
        visit(node.target);
    }
    printLine("value:");
    {
        const IndentGuard _{ indent_ };
        visit(node.value);
    }
}

auto DebugPrinter::operator()(const ast::Process &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";

    if (!node.sensitivity_list.empty()) {
        const auto joined_list = std::ranges::to<std::string>(
          node.sensitivity_list | std::views::join_with(std::string_view{ ', ' }));

        extra += " (";
        extra += joined_list;
        extra += ")";
    }

    emitNodeLike(node, "Process", extra);
    const IndentGuard _{ indent_ };
    visit(node.body);
}

// Sequential Statements

auto DebugPrinter::operator()(const ast::SequentialAssign &node) -> void
{
    emitNodeLike(node, "SequentialAssign", ":=");
    const IndentGuard _{ indent_ };
    printLine("target:");
    {
        const IndentGuard _{ indent_ };
        visit(node.target);
    }
    printLine("value:");
    {
        const IndentGuard _{ indent_ };
        visit(node.value);
    }
}

auto DebugPrinter::operator()(const ast::IfStatement &node) -> void
{
    emitNodeLike(node, "IfStatement", "");
    const IndentGuard _{ indent_ };

    // If branch
    printLine("if:");
    {
        const IndentGuard _{ indent_ };
        printLine("condition:");
        {
            const IndentGuard _{ indent_ };
            visit(node.if_branch.condition);
        }
        printLine("then:");
        {
            const IndentGuard _{ indent_ };
            visit(node.if_branch.body);
        }
    }

    // Elsif branches
    for (const auto &elsif : node.elsif_branches) {
        printLine("elsif:");
        {
            const IndentGuard _{ indent_ };
            printLine("condition:");
            {
                const IndentGuard _{ indent_ };
                visit(elsif.condition);
            }
            printLine("then:");
            {
                const IndentGuard _{ indent_ };
                visit(elsif.body);
            }
        }
    }

    // Else branch
    if (node.else_branch) {
        printLine("else:");
        const IndentGuard _{ indent_ };
        visit(node.else_branch->body);
    }
}

auto DebugPrinter::operator()(const ast::CaseStatement &node) -> void
{
    emitNodeLike(node, "CaseStatement", "");
    const IndentGuard _{ indent_ };

    printLine("selector:");
    {
        const IndentGuard _{ indent_ };
        visit(node.selector);
    }

    for (const auto &when : node.when_clauses) {
        printLine("when:");
        {
            const IndentGuard _{ indent_ };
            printLine("choices:");
            {
                const IndentGuard _{ indent_ };
                visit(when.choices);
            }
            printLine("body:");
            {
                const IndentGuard _{ indent_ };
                visit(when.body);
            }
        }
    }
}

auto DebugPrinter::operator()(const ast::ForLoop &node) -> void
{
    emitNodeLike(node, "ForLoop", "[" + node.iterator + "]");
    const IndentGuard _{ indent_ };

    printLine("range:");
    {
        const IndentGuard _{ indent_ };
        visit(node.range);
    }

    printLine("body:");
    {
        const IndentGuard _{ indent_ };
        visit(node.body);
    }
}

auto DebugPrinter::operator()(const ast::WhileLoop &node) -> void
{
    emitNodeLike(node, "WhileLoop", "");
    const IndentGuard _{ indent_ };

    printLine("condition:");
    {
        const IndentGuard _{ indent_ };
        visit(node.condition);
    }

    printLine("body:");
    {
        const IndentGuard _{ indent_ };
        visit(node.body);
    }
}

} // namespace emit

// NOLINTEND(misc-no-recursion)
