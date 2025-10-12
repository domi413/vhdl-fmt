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
    std::cout << std::string(static_cast<std::uint8_t>(indent_ * 2), ' ');
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

// ---- Nodes ----

void DebugPrinter::visit(const ast::DesignFile &node)
{
    printNode(node, {}, "DesignFile");
    ++indent_;
    walk(node); // recursive traversal handled by BaseVisitor
    --indent_;
}

void DebugPrinter::visit(const ast::Entity &node)
{
    printNode(node, node.name, "Entity");
    ++indent_;
    walk(node); // recurse into generics and ports
    --indent_;
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
    ++indent_;
    walk(node); // recurse into constraints
    --indent_;
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
}

} // namespace emit
