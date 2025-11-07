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

    Doc result = Doc::empty();

    for (auto [index, unit] : std::views::enumerate(node.units)) {
        if (index > 0) {
            // Add blank line between design units
            result = result / Doc::line();
        }
        result = result + visit(unit);
    }

    return result + Doc::line(); // Trailing newline
}

} // namespace emit
