#include "ast/nodes/statements.hpp"
#include "emit/debug_printer.hpp"

#include <ranges>
#include <string>
#include <string_view>

namespace emit {

// Concurrent Statements

auto DebugPrinter::operator()(const ast::ConcurrentSignalAssignmentStatement &node) -> void
{
    emitNodeLike(node, "ConcurrentAssign", "<=");
    const IndentGuard _{ indent_ };
    printLine("target:");
    const IndentGuard _{ indent_ };
    visit(node.target);
    printLine("value:");
    const IndentGuard _{ indent_ };
    visit(node.value);
}

auto DebugPrinter::operator()(const ast::ProcessStatement &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";

    if (!node.sensitivity_list.empty()) {
        const auto joined_list = std::ranges::to<std::string>(
          node.sensitivity_list | std::views::join_with(std::string_view{ ", " }));

        extra += " (";
        extra += joined_list;
        extra += ")";
    }

    emitNodeLike(node, "Process", extra);
    const IndentGuard _{ indent_ };
    visit(node.body);
}

// Sequential Statements

auto DebugPrinter::operator()(const ast::SignalAssignmentStatement &node) -> void
{
    emitNodeLike(node, "SequentialAssign", ":=");
    const IndentGuard _{ indent_ };
    printLine("target:");
    const IndentGuard _{ indent_ };
    visit(node.target);
    printLine("value:");
    const IndentGuard _{ indent_ };
    visit(node.value);
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
            const IndentGuard _{ indent_ };
            visit(elsif.condition);
            printLine("then:");
            const IndentGuard _{ indent_ };
            visit(elsif.body);
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
    const IndentGuard _{ indent_ };
    visit(node.selector);

    for (const auto &when : node.when_clauses) {
        printLine("when:");
        const IndentGuard _{ indent_ };
        printLine("choices:");
        const IndentGuard _{ indent_ };
        visit(when.choices);
        printLine("body:");
        const IndentGuard _{ indent_ };
        visit(when.body);
    }
}

auto DebugPrinter::operator()(const ast::ForLoopStatement &node) -> void
{
    emitNodeLike(node, "ForLoop", "[" + node.iterator + "]");
    const IndentGuard _{ indent_ };

    printLine("range:");
    const IndentGuard _{ indent_ };
    visit(node.range);

    printLine("body:");
    const IndentGuard _{ indent_ };
    visit(node.body);
}

auto DebugPrinter::operator()(const ast::WhileLoopStatement &node) -> void
{
    emitNodeLike(node, "WhileLoop", "");
    const IndentGuard _{ indent_ };

    printLine("condition:");
    const IndentGuard _{ indent_ };
    visit(node.condition);

    printLine("body:");
    const IndentGuard _{ indent_ };
    visit(node.body);
}

auto DebugPrinter::operator()(const ast::WaitStatement &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";

    if (!node.sensitivity_list.empty()) {
        const auto joined_list = std::ranges::to<std::string>(
          node.sensitivity_list | std::views::join_with(std::string_view{ ", " }));
        extra += " ON (" + joined_list + ")";
    }

    emitNodeLike(node, "WaitStatement", extra);
    const IndentGuard _{ indent_ };

    if (node.condition) {
        printLine("until:");
        const IndentGuard _{ indent_ };
        visit(*node.condition);
    }

    if (node.timeout) {
        printLine("for:");
        const IndentGuard _{ indent_ };
        visit(*node.timeout);
    }
}

auto DebugPrinter::operator()(const ast::AssertStatement &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";
    emitNodeLike(node, "AssertStatement", extra);
    const IndentGuard _{ indent_ };

    printLine("condition:");
    const IndentGuard _{ indent_ };
    visit(node.condition);

    if (node.report_expr) {
        printLine("report:");
        const IndentGuard _{ indent_ };
        visit(*node.report_expr);
    }

    if (node.severity_expr) {
        printLine("severity:");
        const IndentGuard _{ indent_ };
        visit(*node.severity_expr);
    }
}

auto DebugPrinter::operator()(const ast::ReportStatement &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";
    emitNodeLike(node, "ReportStatement", extra);
    const IndentGuard _{ indent_ };

    printLine("message:");
    const IndentGuard _{ indent_ };
    visit(node.message);

    if (node.severity_expr) {
        printLine("severity:");
        const IndentGuard _{ indent_ };
        visit(*node.severity_expr);
    }
}

auto DebugPrinter::operator()(const ast::NextStatement &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";
    if (node.loop_label) {
        extra += " → " + *node.loop_label;
    }

    emitNodeLike(node, "NextStatement", extra);

    if (node.condition) {
        const IndentGuard _{ indent_ };
        printLine("when:");
        const IndentGuard _{ indent_ };
        visit(*node.condition);
    }
}

auto DebugPrinter::operator()(const ast::ExitStatement &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";
    if (node.loop_label) {
        extra += " → " + *node.loop_label;
    }

    emitNodeLike(node, "ExitStatement", extra);

    if (node.condition) {
        const IndentGuard _{ indent_ };
        printLine("when:");
        const IndentGuard _{ indent_ };
        visit(*node.condition);
    }
}

auto DebugPrinter::operator()(const ast::ReturnStatement &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";
    emitNodeLike(node, "ReturnStatement", extra);

    if (node.value) {
        const IndentGuard _{ indent_ };
        printLine("value:");
        const IndentGuard _{ indent_ };
        visit(*node.value);
    }
}

auto DebugPrinter::operator()(const ast::ProcedureCallStatement &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";
    extra += " " + node.procedure_name;

    emitNodeLike(node, "ProcedureCall", extra);

    if (node.args) {
        const IndentGuard _{ indent_ };
        printLine("args:");
        const IndentGuard _{ indent_ };
        visit(*node.args);
    }
}

auto DebugPrinter::operator()(const ast::NullStatement &node) -> void
{
    std::string extra = node.label ? "[" + *node.label + "]" : "";
    emitNodeLike(node, "NullStatement", extra);
}

} // namespace emit
