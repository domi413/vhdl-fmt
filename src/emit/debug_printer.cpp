#include "debug_printer.hpp"

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
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

void DebugPrinter::printCommentLines(const std::vector<ast::Trivia> &tv,
                                     std::string_view prefix) const
{
    for (const auto &t : tv) {
        if (const auto *comment = std::get_if<ast::CommentTrivia>(&t)) {
            printIndent();
            if (!prefix.empty()) {
                out_ << prefix;
            }
            out_ << comment->text << '\n';
        }
    }
}

auto DebugPrinter::countNewlines(const std::vector<ast::Trivia> &trailing) -> std::size_t
{
    auto newlines = trailing
                  | std::views::filter([](const ast::Trivia &t) -> bool {
                        return std::holds_alternative<ast::NewlinesTrivia>(t);
                    })
                  | std::views::transform([](const ast::Trivia &t) -> const ast::NewlinesTrivia & {
                        return std::get<ast::NewlinesTrivia>(t);
                    });

    return std::ranges::distance(newlines);
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
        const IndentGuard _{ indent_ };
        printCommentLines(maybe->leading, /*prefix=*/"(^) ");
        printCommentLines(maybe->trailing, /*prefix=*/"(>) ");
    }
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
