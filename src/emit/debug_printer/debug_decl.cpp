#include "ast/nodes/declarations.hpp"
#include "emit/debug_printer.hpp"

#include <cstddef>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

namespace emit {

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

void DebugPrinter::visit(const ast::ConstantDecl &node)
{
    std::ostringstream oss;
    for (size_t i = 0; i < node.names.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << node.names[i];
    }
    oss << " : " << node.type_name;

    emitNodeLike(node, "ConstantDecl", oss.str());

    const IndentGuard _{ indent_ };
    walk(node);
}

} // namespace emit
