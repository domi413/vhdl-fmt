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
                             const std::string &name_override) const
{
    printIndent();
    std::cout << (!name_override.empty() ? name_override : typeid(n).name());
    if (!extra.empty()) {
        std::cout << " [" << extra << "]";
    }
    std::cout << '\n';
}

void DebugPrinter::printComments(const ast::Node &n) const
{
    const auto &maybe_comments = n.tryGetComments();
    if (!maybe_comments.has_value()) {
        return;
    }

    const auto &comments = *maybe_comments;

    // Leading comments
    for (const auto &c : comments.leading) {
        printIndent();
        std::cout << c.text << '\n';
    }

    // Trailing comments
    for (const auto &c : comments.trailing) {
        printIndent();
        std::cout << "(inline) " << c.text << '\n';
    }
}

// ---- Nodes ----

void DebugPrinter::visit(const ast::DesignFile &node)
{
    printNode(node, {}, "DesignFile");
    ++indent;
    printComments(node);
    walk(node);
    --indent;
}

void DebugPrinter::visit(const ast::Entity &node)
{
    printNode(node, node.name, "Entity");
    ++indent;
    printComments(node);
    walk(node);
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

    printNode(node, info, "Generic");
    ++indent;
    printComments(node);
    --indent;
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

    printNode(node, oss.str(), "Port");
    ++indent;
    printComments(node);
    walk(node);
    --indent;
}

void DebugPrinter::visit(const ast::Range &node)
{
    printIndent();
    std::cout
      << "Range ["
      << node.left_expr
      << " "
      << node.direction
      << " "
      << node.right_expr
      << "]\n";
    ++indent;
    printComments(node);
    --indent;
}

} // namespace emit
