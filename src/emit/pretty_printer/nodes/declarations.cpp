#include "ast/nodes/declarations.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/doc_utils.hpp"

namespace emit {

auto PrettyPrinter::operator()(const ast::GenericParam &node) -> Doc
{
    // <name> : <type> [:= <default>]
    // Multiple names: name1, name2 : type

    // Names (joined with comma and space)
    auto name_docs = toDocVector(node.names, [](const auto &name) { return Doc::text(name); });
    Doc result = joinDocs(name_docs, Doc::text(",") & Doc::empty(), false);

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

    // Names (joined with comma and space)
    auto name_docs = toDocVector(node.names, [](const auto &name) { return Doc::text(name); });
    Doc result = joinDocs(name_docs, Doc::text(",") & Doc::empty(), false);

    // Mode and type
    result = result & Doc::text(":") & Doc::text(node.mode) & Doc::text(node.type_name);

    // Constraint (e.g., (7 downto 0) or range 0 to 255)
    if (node.constraint.has_value()) {
        result = result + visit(node.constraint.value());
    }

    // Default value
    if (node.default_expr.has_value()) {
        result = result & Doc::text(":=") & visit(node.default_expr.value());
    }

    return result.group();
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::SignalDecl &node) -> Doc
{
    // TODO(vedivad): Implement signal declaration printing
    return Doc::text("-- signal");
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::ConstantDecl &node) -> Doc
{
    // TODO(vedivad): Implement constant declaration printing
    return Doc::text("-- constant");
}

} // namespace emit
