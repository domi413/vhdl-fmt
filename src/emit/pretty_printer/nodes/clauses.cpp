#include "ast/nodes/design_units.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/doc_utils.hpp"

#include <vector>

namespace emit {

auto PrettyPrinter::operator()(const ast::GenericClause &node) const -> Doc
{
    if (node.generics.empty()) {
        return Doc::empty();
    }

    const Doc opener = Doc::text("generic") & Doc::text("(");
    const Doc closer = Doc::text(");");

    const std::vector<Doc> params = toDocVectorVisit(node.generics, *this);

    // TODO(vedivad): Implement table combinator for alignment
    // if (config().port_map.align_signals) {

    // }

    const Doc result = joinDocs(params, Doc::text(";") + Doc::line(), false);
    return Doc::bracket(opener, result, closer).group();
}

auto PrettyPrinter::operator()(const ast::PortClause &node) const -> Doc
{
    if (node.ports.empty()) {
        return Doc::empty();
    }

    const Doc opener = Doc::text("port") & Doc::text("(");
    const Doc closer = Doc::text(");");

    const auto ports = toDocVectorVisit(node.ports, *this);

    // if (config().port_map.align_signals) {

    // }

    const Doc result = joinDocs(ports, Doc::text(";") + Doc::line(), false);
    return Doc::bracket(opener, result, closer).group();
}

} // namespace emit
