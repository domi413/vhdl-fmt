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
    for (std::string_view sv :
         tv | std::views::filter([](const ast::Trivia &t) {
             return std::holds_alternative<ast::CommentTrivia>(t);
         }) | std::views::transform([](const ast::Trivia &t) -> std::string_view {
             return std::get<ast::CommentTrivia>(t).text;
         })) {
        printIndent();
        if (!prefix.empty()) {
            out << prefix;
        }
        out << sv << '\n';
    }
}

auto DebugPrinter::countNewlines(const std::vector<ast::Trivia> &trailing) -> std::size_t
{
    std::size_t total = 0;
    for (const auto &t : trailing) {
        std::visit(
          [&](const auto &val) {
              using T = std::decay_t<decltype(val)>;
              if constexpr (std::is_same_v<T, ast::NewlinesTrivia>) {
                  total += val.breaks;
              }
          },
          t);
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
        const IndentGuard _{ indent };
        printCommentLines(maybe->leading, /*prefix=*/"(^) ");
        printCommentLines(maybe->trailing, /*prefix=*/"(>) ");
    }
}

// ---- Visitor methods ----

void DebugPrinter::visit(const ast::DesignFile &node)
{
    emitNodeLike(node, "DesignFile", /*extra=*/"");
    const IndentGuard _{ indent };
    walk(node);
}

void DebugPrinter::visit(const ast::Entity &node)
{
    emitNodeLike(node, "Entity", node.name);

    const IndentGuard _{ indent };
    // Children sections
    printLine("Generics:");
    {
        const IndentGuard _{ indent };
        for (const auto &g : node.generics) {
            if (g) {
                g->accept(*this);
            }
        }
    }
    printLine("Ports:");
    {
        const IndentGuard _{ indent };
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

    const IndentGuard _{ indent };
    walk(node);
}

void DebugPrinter::visit(const ast::Range &node)
{
    std::ostringstream oss;
    oss << node.left_expr << ' ' << node.direction << ' ' << node.right_expr;
    emitNodeLike(node, "Range", oss.str());
}

} // namespace emit
