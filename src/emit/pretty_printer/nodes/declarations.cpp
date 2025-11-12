#include "ast/nodes/declarations.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <ranges>
#include <string>

namespace emit {

auto PrettyPrinter::operator()(const ast::GenericParam &node) const -> Doc
{
    // <name> : <type> [:= <default>]

    const std::string names = node.names
                            | std::views::join_with(std::string_view{ ", " })
                            | std::ranges::to<std::string>();

    Doc result = Doc::alignText(names) & Doc::text(":") & Doc::text(node.type_name);

    if (node.default_expr) {
        result &= Doc::text(":=") & visit(node.default_expr.value());
    }

    return result;
}

auto PrettyPrinter::operator()(const ast::Port &node) const -> Doc
{
    // <name> : <mode> <type> [:= <default>]

    const std::string names = node.names
                            | std::views::join_with(std::string_view{ ", " })
                            | std::ranges::to<std::string>();

    Doc result
      = Doc::alignText(names) & Doc::text(":") & Doc::text(node.mode) & Doc::text(node.type_name);

    // Constraint (e.g., (7 downto 0) or range 0 to 255)
    if (node.constraint) {
        result += visit(node.constraint.value());
    }

    if (node.default_expr) {
        result &= Doc::text(":=") & visit(node.default_expr.value());
    }

    return result;
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::SignalDecl &node) const -> Doc
{
    // TODO(vedivad): Implement signal declaration printing
    return Doc::text("-- signal");
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::ConstantDecl &node) const -> Doc
{
    // TODO(vedivad): Implement constant declaration printing
    return Doc::text("-- constant");
}

} // namespace emit
