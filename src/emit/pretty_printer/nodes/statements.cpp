#include "ast/nodes/statements.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

namespace emit {

auto PrettyPrinter::operator()(const ast::ConcurrentAssign &node) -> Doc
{
    return visit(node.target) & Doc::text("<=") & visit(node.value) + Doc::text(";");
}

auto PrettyPrinter::operator()(const ast::SequentialAssign &node) -> Doc
{
    return visit(node.target) & Doc::text(":=") & visit(node.value) + Doc::text(";");
}

auto PrettyPrinter::operator()(const ast::IfStatement &node) -> Doc
{
    // TODO(user): Implement if statement printing
    (void)node;
    return Doc::text("-- if statement");
}

auto PrettyPrinter::operator()(const ast::CaseStatement &node) -> Doc
{
    // TODO(user): Implement case statement printing
    (void)node;
    return Doc::text("-- case statement");
}

auto PrettyPrinter::operator()(const ast::Process &node) -> Doc
{
    // TODO(user): Implement process printing
    (void)node;
    return Doc::text("-- process");
}

auto PrettyPrinter::operator()(const ast::ForLoop &node) -> Doc
{
    // TODO(user): Implement for loop printing
    (void)node;
    return Doc::text("-- for loop");
}

auto PrettyPrinter::operator()(const ast::WhileLoop &node) -> Doc
{
    // TODO(user): Implement while loop printing
    (void)node;
    return Doc::text("-- while loop");
}

} // namespace emit
