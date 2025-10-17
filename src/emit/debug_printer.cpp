#include "debug_printer.hpp"

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/entity.hpp"

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
    const std::size_t newlines = countNewlines(node.leading());
    printNodeHeader(node, extra, pretty_name, newlines);

    const IndentGuard _{ indent_ };

    // Leading comments
    std::vector<ast::Comments> leading_comments;
    for (const auto &t : node.leading()) {
        if (const auto *c = std::get_if<ast::Comments>(&t)) {
            leading_comments.push_back(*c);
        }
    }

    printCommentLines(leading_comments, "(^) ");
    printCommentLines(node.trailing(), "(>) ");
}

// ---- Visitor methods ----

void DebugPrinter::visit(const ast::DesignFile &node)
{
    emitNodeLike(node, "DesignFile", "");
    const IndentGuard _{ indent_ };
    walk(node);
}

void DebugPrinter::visit(const ast::Entity &node)
{
    emitNodeLike(node, "Entity", node.name);
    const IndentGuard _{ indent_ };

    if (node.generic_clause) {
        node.generic_clause->accept(*this);
    }
    if (node.port_clause) {
        node.port_clause->accept(*this);
    }

    if (!node.decls.empty()) {
        printLine("Declarations:");
        const IndentGuard _{ indent_ };
        for (const auto &d : node.decls) {
            if (d) {
                d->accept(*this);
            }
        }
    }

    if (!node.stmts.empty()) {
        printLine("Statements:");
        const IndentGuard _{ indent_ };
        for (const auto &s : node.stmts) {
            if (s) {
                s->accept(*this);
            }
        }
    }
}

void DebugPrinter::visit(const ast::Architecture &node)
{
    emitNodeLike(node, "Architecture", node.name + " of " + node.entity_name);
    const IndentGuard _{ indent_ };
    walk(node);
}

void DebugPrinter::visit(const ast::GenericClause &node)
{
    emitNodeLike(node, "GenericClause", "");
    const IndentGuard _{ indent_ };
    walk(node);
}

void DebugPrinter::visit(const ast::PortClause &node)
{
    emitNodeLike(node, "PortClause", "");
    const IndentGuard _{ indent_ };
    walk(node);
}

void DebugPrinter::visit(const ast::GenericParam &node)
{
    std::ostringstream oss;
    oss << std::ranges::to<std::string>(node.names
                                        | std::views::join_with(std::string_view{ ", " }));
    oss << " : " << node.type_name;

    emitNodeLike(node, "GenericParam", oss.str());

    const IndentGuard _{ indent_ };
    walk(node);
}

void DebugPrinter::visit(const ast::Port &node)
{
    std::ostringstream oss;
    oss << std::ranges::to<std::string>(node.names
                                        | std::views::join_with(std::string_view{ ", " }));

    if (!node.mode.empty() || !node.type_name.empty()) {
        oss << " :";
        if (!node.mode.empty()) {
            oss << ' ' << node.mode;
        }
        if (!node.type_name.empty()) {
            oss << ' ' << node.type_name;
        }
    }

    emitNodeLike(node, "Port", oss.str());

    const IndentGuard _{ indent_ };
    walk(node);
}

void DebugPrinter::visit(const ast::SignalDecl &node)
{
    std::ostringstream oss;
    for (size_t i = 0; i < node.names.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << node.names[i];
    }
    oss << " : " << node.type_name;
    if (node.has_bus_kw) {
        oss << " [bus]";
    }
    emitNodeLike(node, "SignalDecl", oss.str());

    const IndentGuard _{ indent_ };
    walk(node);
}

void DebugPrinter::visit(const ast::Range &node)
{
    emitNodeLike(node, "Range", node.direction);
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

} // namespace emit
