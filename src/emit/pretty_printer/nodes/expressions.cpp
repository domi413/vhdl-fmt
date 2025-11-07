#include "ast/nodes/expressions.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

namespace emit {

auto PrettyPrinter::operator()(const ast::TokenExpr &node) -> Doc
{
    return Doc::text(node.text);
}

auto PrettyPrinter::operator()(const ast::GroupExpr & /*node*/) -> Doc
{
    // TODO(dyb): Implement group expression printing
    return Doc::text("(group)");
}

auto PrettyPrinter::operator()(const ast::UnaryExpr &node) -> Doc
{
    return Doc::text(node.op) & visit(*node.value);
}

auto PrettyPrinter::operator()(const ast::BinaryExpr &node) -> Doc
{
    return visit(*node.left) & Doc::text(node.op) & visit(*node.right);
}

auto PrettyPrinter::operator()(const ast::ParenExpr &node) -> Doc
{
    return Doc::text("(") + visit(*node.inner) + Doc::text(")");
}

auto PrettyPrinter::operator()(const ast::CallExpr &node) -> Doc
{
    return visit(*node.callee) + Doc::text("(") + visit(*node.args) + Doc::text(")");
}

} // namespace emit
