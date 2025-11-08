#include "ast/nodes/declarations.hpp"
#include "emit/debug_printer.hpp"

#include <ranges>
#include <sstream>
#include <string>

namespace emit {

auto DebugPrinter::operator()(const ast::InterfaceConstantDecl &node) -> void
{
    std::ostringstream oss;
    oss << std::ranges::to<std::string>(node.names
                                        | std::views::join_with(std::string_view{ ", " }));
    oss << " : " << node.type_name;

    emitNodeLike(node, "GenericParam", oss.str());

    const IndentGuard _{ indent_ };
    visit(node.default_expr);
}

auto DebugPrinter::operator()(const ast::InterfacePortDecl &node) -> void
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

auto DebugPrinter::operator()(const ast::AliasDecl &node) -> void
{
    std::ostringstream oss;
    oss << node.name;

    if (node.type_indication) {
        oss << " : " << *node.type_indication;
    }

    oss << " is";

    emitNodeLike(node, "AliasDecl", oss.str());

    const IndentGuard _{ indent_ };
    visit(node.target);
}

} // namespace emit
