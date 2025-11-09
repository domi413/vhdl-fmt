#include "ast/nodes/design_file.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <ranges>

namespace emit {

auto PrettyPrinter::operator()(const ast::DesignFile &node) const -> Doc
{
    if (node.units.empty()) {
        return Doc::empty();
    }

    Doc result = Doc::empty();

    for (const auto [i, unit] : std::views::enumerate(node.units)) {
        if (i > 0) {
            result /= Doc::line(); // Blank line separator
        }
        result += visit(unit);
    }

    return result + Doc::line(); // Trailing newline
}

} // namespace emit
