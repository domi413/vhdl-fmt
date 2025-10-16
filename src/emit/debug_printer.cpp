#include "debug_printer.hpp"

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string_view>
#include <typeinfo>
#include <variant>
#include <vector>

namespace emit {

// ---- Helper methods ----

void DebugPrinter::printIndent() const
{
    out_ << std::string(static_cast<std::uint8_t>(indent_ * 2), ' ');
}

void DebugPrinter::printLine(std::string_view s) const
{
    printIndent();
    out_ << s << '\n';
}

void DebugPrinter::printNodeHeader(const ast::Node &n,
                                   const std::string &extra,
                                   std::string_view name_override,
                                   std::size_t trailing_breaks) const
{
    printIndent();
    out_ << (!name_override.empty() ? std::string{ name_override } : typeid(n).name());
    if (!extra.empty()) {
        out_ << " [" << extra << "]";
    }
    if (trailing_breaks > 0U) {
        out_ << " (" << trailing_breaks << R"([\n]))";
    }
    out_ << '\n';
}

void DebugPrinter::printCommentLines(const std::vector<ast::Comments> &comments,
                                     std::string_view prefix) const
{
    for (const auto &comment : comments) {
        printIndent();
        if (!prefix.empty()) {
            out_ << prefix;
        }
        out_ << comment.text << '\n';
    }
}

auto DebugPrinter::countNewlines(const std::vector<ast::Trivia> &leading) -> std::size_t
{
    std::size_t total = 0;
    for (const auto &t : leading) {
        if (const auto *nl = std::get_if<ast::Newlines>(&t)) {
            total += nl->breaks;
        }
    }
    return total;
}

template<class NodeT>
void DebugPrinter::emitNodeLike(const NodeT &node,
                                std::string_view pretty_name,
                                const std::string &extra)
{
    const auto &trivia = node.tryGetTrivia();

    // LEFT-STICK: show blank lines owned by this node (leading)
    const std::size_t newlines = countNewlines(node.leading());
    printNodeHeader(node, extra, pretty_name, newlines);

    const IndentGuard _{ indent_ };

    // Print leading comments (full-line), then trailing inline comments
    std::vector<ast::Comments> leading_comments;
    leading_comments.reserve(node.leading().size());
    for (const auto &t : node.leading()) {
        if (const auto *c = std::get_if<ast::Comments>(&t)) {
            leading_comments.push_back(*c);
        }
    }

    printCommentLines(leading_comments, /*prefix=*/"(^) ");
    printCommentLines(node.trailing(), /*prefix=*/"(>) ");
}

// ---- Visitor methods ----

void DebugPrinter::visit(const ast::DesignFile &node)
{
    emitNodeLike(node, "DesignFile", /*extra=*/"");
    const IndentGuard _{ indent_ };
    walk(node);
}

void DebugPrinter::visit(const ast::Entity &node)
{
    emitNodeLike(node, "Entity", node.name);

    const IndentGuard _{ indent_ };
    // Children sections
    printLine("Generics:");
    {
        const IndentGuard _{ indent_ };
        for (const auto &g : node.generics) {
            if (g) {
                g->accept(*this);
            }
        }
    }
    printLine("Ports:");
    {
        const IndentGuard _{ indent_ };
        for (const auto &p : node.ports) {
            if (p) {
                p->accept(*this);
            }
        }
    }
}

void DebugPrinter::visit(const ast::GenericParam &node)
{
    auto names = node.names | std::views::join_with(std::string_view{ ", " });
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

    auto names = node.names | std::views::join_with(std::string_view{ ", " });
    oss << std::ranges::to<std::string>(names);

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

    const IndentGuard _{ indent_ };
    walk(node);
}

void DebugPrinter::visit(const ast::Range &node)
{
    std::ostringstream oss;
    oss << node.left_expr << ' ' << node.direction << ' ' << node.right_expr;
    emitNodeLike(node, "Range", oss.str());
}

} // namespace emit
