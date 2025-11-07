#include "ast/nodes/declarations.hpp"

#include "emit/pretty_printer.hpp"

namespace emit {

auto PrettyPrinter::operator()(const ast::GenericParam &node) -> Doc
{
    // <name> : <type> [:= <default>]
    // Multiple names: name1, name2 : type
    Doc result = Doc::empty();

    // Names
    bool first = true;
    for (const auto &name : node.names) {
        if (!first) {
            result = result + Doc::text(",") & Doc::text(name);
        } else {
            result = Doc::text(name);
        }
        first = false;
    }

    // Type
    result = result & Doc::text(":") & Doc::text(node.type_name);

    // Default value
    if (node.default_expr.has_value()) {
        result = result & Doc::text(":=") & visit(node.default_expr.value());
    }

    return result.group();
}

auto PrettyPrinter::operator()(const ast::Port &node) -> Doc
{
    // <name> : <mode> <type> [:= <default>]
    // Multiple names: name1, name2 : in type
    Doc result = Doc::empty();

    // Names
    bool first = true;
    for (const auto &name : node.names) {
        if (!first) {
            result = result + Doc::text(",") & Doc::text(name);
        } else {
            result = Doc::text(name);
        }
        first = false;
    }

    // Mode and type
    result = result & Doc::text(":") & Doc::text(node.mode) & Doc::text(node.type_name);

    // Constraints (e.g., (7 downto 0))
    if (!node.constraints.empty()) {
        Doc constraints = Doc::text("(");
        bool first_constraint = true;
        for (const auto &constraint : node.constraints) {
            if (!first_constraint) {
                constraints = constraints + Doc::text(",") & visit(constraint);
            } else {
                constraints = constraints + visit(constraint);
            }
            first_constraint = false;
        }
        constraints = constraints + Doc::text(")");
        result = result + constraints;
    }

    // Default value
    if (node.default_expr.has_value()) {
        result = result & Doc::text(":=") & visit(node.default_expr.value());
    }

    return result.group();
}

auto PrettyPrinter::operator()(const ast::SignalDecl &node) -> Doc
{
    // TODO(user): Implement signal declaration printing
    (void)node;
    return Doc::text("-- signal");
}

auto PrettyPrinter::operator()(const ast::ConstantDecl &node) -> Doc
{
    // TODO(user): Implement constant declaration printing
    (void)node;
    return Doc::text("-- constant");
}

} // namespace emit
