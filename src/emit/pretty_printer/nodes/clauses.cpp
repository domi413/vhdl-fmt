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

    const auto params
      = toDocVector(node.generics, [this](const auto &param) { return visit(param); });

    const Doc result = joinDocs(params, Doc::text(";") + Doc::line(), false);
    const Doc aligned_result = Doc::align(result);

    return Doc::group(Doc::bracket(opener, aligned_result, closer));
}

auto PrettyPrinter::operator()(const ast::PortClause &node) const -> Doc
{
    if (std::ranges::empty(node.ports)) {
        return Doc::empty();
    }

    const Doc opener = Doc::text("port") & Doc::text("(");
    const Doc closer = Doc::text(");");

    const auto ports = toDocVector(node.ports, [this](const auto &port) { return visit(port); });

    const Doc result = joinDocs(ports, Doc::text(";") + Doc::line(), false);
    const Doc aligned_result = Doc::align(result);

    return Doc::group(Doc::bracket(opener, aligned_result, closer));
}

} // namespace emit
