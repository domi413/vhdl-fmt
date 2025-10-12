#include "debug_printer.hpp"

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"

#include <cstdint>
#include <iostream>
#include <ranges>
#include <sstream>
#include <typeinfo>

namespace emit {

void DebugPrinter::printIndent() const
{
    std::cout << std::string(static_cast<std::uint8_t>(indent * 2), ' ');
}

void DebugPrinter::printNode(const ast::Node &n,
                             const std::string &extra,
                             const std::string &name_override,
                             const std::string &inline_suffix) const
{
    printIndent();
    std::cout << (!name_override.empty() ? name_override : typeid(n).name());
    if (!extra.empty()) {
        std::cout << " [" << extra << "]";
    }
    if (!inline_suffix.empty()) {
        std::cout << " " << inline_suffix;
    }
    std::cout << '\n';
}

void DebugPrinter::printTriviaLines(const std::vector<ast::Trivia> &tv) const
{
    for (const auto &t : tv) {
        if (t.kind == ast::Trivia::Kind::comment) {
            printIndent();
            std::cout << t.text << '\n';
        }
    }
}

auto DebugPrinter::buildInlineSuffix(const std::vector<ast::Trivia> &trailing) const -> std::string
{
    std::ostringstream oss;
    bool first_comment = true;
    std::size_t newline_breaks = 0;

    for (const auto &t : trailing) {
        if (t.kind == ast::Trivia::Kind::comment) {
            if (!first_comment) {
                oss << ' ';
            }
            // Text already includes the VHDL prefix ("-- ...").
            oss << t.text;
            first_comment = false;
        } else {
            newline_breaks += t.breaks;
        }
    }

    if (newline_breaks > 0U) {
        if (!first_comment) {
            oss << ' ';
        }
        oss << '(' << newline_breaks << R"([\n]))";
    }

    return oss.str();
}

// ---- Generic emission helper ----

template<class NodeT>
void DebugPrinter::emitNodeLike(const NodeT &node,
                                std::string_view pretty_name,
                                const std::string &extra)
{
    const auto &maybe = node.tryGetComments();
    if (maybe.has_value()) {
        // Leading trivia above the node.
        printTriviaLines(maybe->leading);
    }

    // Build inline suffix from trailing trivia (inline comments + "(N[\n])").
    const std::string inline_suffix
      = maybe.has_value() ? buildInlineSuffix(maybe->trailing) : std::string{};

    // Print header line.
    printNode(node, extra, std::string(pretty_name), inline_suffix);

    // Do NOT emit extra blank lines after the header — the trailing newline count
    // is already shown inline via "(N[\n])".
}

// ---- Nodes ----

void DebugPrinter::visit(const ast::DesignFile &node)
{
    // DesignFile typically has no inline comments; use the helper for consistency.
    emitNodeLike(node, "DesignFile", /*extra=*/"");

    ++indent;
    walk(node);
    --indent;
}

void DebugPrinter::visit(const ast::Entity &node)
{
    emitNodeLike(node, "Entity", node.name);

    ++indent;
    std::cout << std::string(static_cast<std::uint8_t>(indent * 2), ' ') << "Generics:\n";
    ++indent;
    dispatchAll(node.generics);
    --indent;
    std::cout << std::string(static_cast<std::uint8_t>(indent * 2), ' ') << "Ports:\n";
    ++indent;
    dispatchAll(node.ports);
    --indent;
}

void DebugPrinter::visit(const ast::GenericParam &node)
{
    std::string info;
    for (const auto &n : node.names) {
        if (!info.empty()) {
            info += ", ";
        }
        info += n;
    }
    info += " : " + node.type;
    if (node.init.has_value()) {
        info += " := " + *node.init;
    }

    emitNodeLike(node, "Generic", info);
}

void DebugPrinter::visit(const ast::Port &node)
{
    std::ostringstream oss;
    for (const auto &[i, name] : node.names | std::views::enumerate) {
        if (i > 0) {
            oss << ", ";
        }
        oss << name;
    }
    if (!node.mode.empty() || !node.type.empty()) {
        oss << " :";
        if (!node.mode.empty()) {
            oss << " " << node.mode;
        }
        if (!node.type.empty()) {
            oss << " " << node.type;
        }
    }

    emitNodeLike(node, "Port", oss.str());

    ++indent;
    // Ports may have children (e.g., ranges)
    walk(node);
    --indent;
}

void DebugPrinter::visit(const ast::Range &node)
{
    std::ostringstream oss;
    oss << node.left_expr << ' ' << node.direction << ' ' << node.right_expr;
    emitNodeLike(node, "Range", oss.str());
}

} // namespace emit
