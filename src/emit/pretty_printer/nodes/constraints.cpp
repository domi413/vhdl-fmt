#include "ast/nodes/expressions.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

namespace emit {

auto PrettyPrinter::operator()(const ast::IndexConstraint &node) -> Doc
{
    // Index constraints are parenthesized ranges: (7 downto 0) or (0 to 15, 0 to 7)
    // The GroupExpr visitor already adds the parentheses
    return visit(node.ranges);
}

auto PrettyPrinter::operator()(const ast::RangeConstraint &node) -> Doc
{
    // Range constraints have the RANGE keyword: range 0 to 255
    return Doc::text("range") & visit(node.range);
}

} // namespace emit
