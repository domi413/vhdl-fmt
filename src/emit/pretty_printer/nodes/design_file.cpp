#include "ast/nodes/design_file.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/doc_utils.hpp"

#include <ranges>

namespace emit {

auto PrettyPrinter::operator()(const ast::DesignFile &node) const -> Doc
{
    if (std::ranges::empty(node.units)) {
        return Doc::empty();
    }

    const auto unit_docs
      = toDocVector(node.units, [this](const auto &unit) { return visit(unit); });
    const auto result = joinDocs(unit_docs, Doc::line(), false);

    return result + Doc::line(); // Trailing newline
}

} // namespace emit
