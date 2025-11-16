#include "ast/nodes/design_units.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/doc_utils.hpp"

#include <ranges>

namespace emit {

auto PrettyPrinter::operator()(const ast::GenericClause &node) const -> Doc
{
    if (std::ranges::empty(node.generics)) {
        return Doc::empty();
    }

    const Doc opener = Doc::text("generic") & Doc::text("(");
    const Doc closer = Doc::text(");");

    const auto params = toDocVector(node.generics, [this](const auto &p) { return visit(p); });

    const Doc doc = joinDocs(params, Doc::line(), false);
    const Doc result = Doc::align(doc);

    // This withTrivia handles trivia for the *entire clause*
    return withTrivia(node, Doc::group(Doc::bracket(opener, result, closer)));
}

auto PrettyPrinter::operator()(const ast::PortClause &node) const -> Doc
{
    if (std::ranges::empty(node.ports)) {
        return Doc::empty();
    }

    const Doc opener = Doc::text("port") & Doc::text("(");
    const Doc closer = Doc::text(");");

    const auto ports = toDocVector(node.ports, [this](const auto &p) { return visit(p); });

    const Doc doc = joinDocs(ports, Doc::line(), false);
    const Doc result = Doc::align(doc);

    return withTrivia(node, Doc::group(Doc::bracket(opener, result, closer)));
}

} // namespace emit
