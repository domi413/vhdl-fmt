#include "debug_printer.hpp"

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"

#include <iostream>
#include <ranges>
#include <sstream>
#include <typeinfo>

namespace emit {

// --- small helpers ---

void DebugPrinter::printIndent() const
{
    out << std::string(static_cast<std::uint8_t>(indent * 2), ' ');
}

void DebugPrinter::printLine(std::string_view s) const
{
    printIndent();
    out << s << '\n';
}

void DebugPrinter::printNodeHeader(const ast::Node &n,
                                   const std::string &extra,
                                   std::string_view name_override,
                                   std::size_t trailing_breaks) const
{
    printIndent();
    out << (!name_override.empty() ? std::string{ name_override } : typeid(n).name());
    if (!extra.empty()) {
        out << " [" << extra << "]";
    }
    if (trailing_breaks > 0U) {
        out << " (" << trailing_breaks << R"([\n]))";
    }
    out << '\n';
}

void DebugPrinter::printCommentLines(const std::vector<ast::Trivia> &tv,
                                     std::string_view prefix) const
{
    for (const auto &t : tv) {
        if (t.kind == ast::Trivia::Kind::comment) {
            printIndent();
            if (!prefix.empty()) {
                out << prefix;
            }
            out << t.text << '\n';
        }
    }
}

auto DebugPrinter::countNewlines(const std::vector<ast::Trivia> &trailing) -> std::size_t
{
    std::size_t total = 0;
    for (const auto &t : trailing) {
        if (t.kind == ast::Trivia::Kind::newlines) {
            total += t.breaks;
        }
    }
    return total;
}

template<class NodeT>
void DebugPrinter::emitNodeLike(const NodeT &node,
                                std::string_view pretty_name,
                                const std::string &extra)
{
    const auto &maybe = node.tryGetComments();
    const std::size_t nl = maybe.has_value() ? countNewlines(maybe->trailing) : 0U;

    // 1) Header with only newline count.
    printNodeHeader(node, extra, pretty_name, nl);

    // 2) Under the node, show leading comments, then trailing inline comments.
    if (maybe.has_value()) {
        IndentGuard _{ indent };
        printCommentLines(maybe->leading, /*prefix=*/"(^) ");
        printCommentLines(maybe->trailing, /*prefix=*/"(>) ");
    }
}

// ---- Visitors ----

void DebugPrinter::visit(const ast::DesignFile &node)
{
    emitNodeLike(node, "DesignFile", /*extra=*/"");
    IndentGuard _{ indent };
    walk(node);
}

void DebugPrinter::visit(const ast::Entity &node)
{
    emitNodeLike(node, "Entity", node.name);

    IndentGuard _{ indent };
    // Children sections
    printLine("Generics:");
    {
        IndentGuard _{ indent };
        for (const auto &g : node.generics) {
            if (g != nullptr) {
                g->accept(*this);
            }
        }
    }
    printLine("Ports:");
    {
        IndentGuard _{ indent };
        for (const auto &p : node.ports) {
            if (p != nullptr) {
                p->accept(*this);
            }
        }
    }
}

void DebugPrinter::visit(const ast::GenericParam &node)
{
    auto names = node.names | (std::views::join_with(std::string_view{ ", " }));
    auto info = std::ranges::to<std::string>(names);
    info += " : " + node.type;
    if (node.init.has_value()) {
        info += " := " + *node.init;
    }

    emitNodeLike(node, "Generic", info);
}

void DebugPrinter::visit(const ast::Port &node)
{
    std::ostringstream oss;

    auto names = node.names | (std::views::join_with(std::string_view{ ", " }));
    oss << std::ranges::to<std::string>(names);

    // mode/type (if present)
    if (!node.mode.empty() || !node.type.empty()) {
        oss << " :";
        if (!node.mode.empty()) {
            oss << ' ' << node.mode;
        }
        if (!node.type.empty()) {
            oss << ' ' << node.type;
        }
    }

    emitNodeLike(node, "Port", oss.str());

    IndentGuard _{ indent };
    walk(node);
}

void DebugPrinter::visit(const ast::Range &node)
{
    std::ostringstream oss;
    oss << node.left_expr << ' ' << node.direction << ' ' << node.right_expr;
    emitNodeLike(node, "Range", oss.str());
}

} // namespace emit
