#include "ast/nodes/declarations.hpp"
#include "emit/debug_printer.hpp"

#include <cstddef>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

namespace emit {

auto DebugPrinter::operator()(const ast::GenericParam &node) -> void
{
    std::ostringstream oss;
    oss << std::ranges::to<std::string>(node.names
                                        | std::views::join_with(std::string_view{ ", " }));
    oss << " : " << node.type_name;

    emitNodeLike(node, "GenericParam", oss.str());

    const IndentGuard _{ indent_ };
    visit(node.default_expr);
}

auto DebugPrinter::operator()(const ast::Port &node) -> void
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
    visit(node.default_expr);
    visit(node.constraints);
}

auto DebugPrinter::operator()(const ast::SignalDecl &node) -> void
{
    std::ostringstream oss{};
    oss << std::ranges::to<std::string>(node.names
                                        | std::views::join_with(std::string_view{ ", " }));
    oss << " : " << node.type_name;
    if (node.has_bus_kw) {
        oss << " [bus]";
    }
    emitNodeLike(node, "SignalDecl", oss.str());

    const IndentGuard _{ indent_ };
    visit(node.init_expr);
    visit(node.constraints);
}

auto DebugPrinter::operator()(const ast::ConstantDecl &node) -> void
{
    std::ostringstream oss;
    oss << std::ranges::to<std::string>(node.names
                                        | std::views::join_with(std::string_view{ ", " }));
    oss << " : " << node.type_name;

    emitNodeLike(node, "ConstantDecl", oss.str());

    const IndentGuard _{ indent_ };
    visit(node.init_expr);
}

} // namespace emit
