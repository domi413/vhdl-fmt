#include "ast/nodes/declarations.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/doc_utils.hpp"

namespace emit {

auto PrettyPrinter::operator()(const ast::GenericParam &node) const -> Doc
{
    // <name> : <type> [:= <default>]
    // Multiple names: name1, name2 : type

    // Names (joined with comma and space)
    auto name_docs = toDocVector(node.names, [](const auto &name) { return Doc::text(name); });
    Doc result = joinDocs(name_docs, Doc::text(", "), false);

    // Type
    result &= Doc::text(":") & Doc::text(node.type_name);

    // Default value
    if (node.default_expr) {
        result &= Doc::text(":=") & visit(node.default_expr.value());
    }

    return result;
}

auto PrettyPrinter::operator()(const ast::Port &node) const -> Doc
{
    // <name> : <mode> <type> [:= <default>]
    // Multiple names: name1, name2 : in type

    // Names (joined with comma and space)
    auto name_docs = toDocVector(node.names, [](const auto &name) { return Doc::text(name); });
    Doc result = joinDocs(name_docs, Doc::text(", "), false);

    // Mode and type
    result &= Doc::text(":") & Doc::text(node.mode) & Doc::text(node.type_name);

    // Constraint (e.g., (7 downto 0) or range 0 to 255)
    if (node.constraint) {
        result += visit(node.constraint.value());
    }

    // Default value
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

auto PrettyPrinter::operator()([[maybe_unused]] const ast::AliasDecl &node) const -> Doc
{
    // TODO: Implement alias declaration printing
    return Doc::text("-- alias");
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::TypeDecl &node) const -> Doc
{
    // TODO: Implement type declaration printing
    return Doc::text("-- type");
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::SubtypeDecl &node) const -> Doc
{
    // TODO: Implement subtype declaration printing
    return Doc::text("-- subtype");
}

} // namespace emit
