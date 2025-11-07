#include "ast/nodes/design_file.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <ranges>

namespace emit {

auto PrettyPrinter::operator()(const ast::DesignFile &node) -> Doc
{
    if (node.units.empty()) {
        return Doc::empty();
    }

    auto docs
      = node.units | std::views::transform([this](const auto &unit) { return visit(unit); });

    Doc result = Doc::empty();
    for (auto [index, doc] : std::views::enumerate(docs)) {
        if (index > 0) {
            // Add blank line between design units
            result = result / Doc::line();
        }
        result = result + doc;
    }

    return result + Doc::line(); // Trailing newline
}

} // namespace emit
