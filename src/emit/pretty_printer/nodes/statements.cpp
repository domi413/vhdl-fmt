#include "ast/nodes/statements.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

namespace emit {

auto PrettyPrinter::operator()(const ast::ConcurrentAssign &node) const -> Doc
{
    return visit(node.target) & Doc::text("<=") & visit(node.value) + Doc::text(";");
}

auto PrettyPrinter::operator()(const ast::SequentialAssign &node) const -> Doc
{
    return visit(node.target) & Doc::text(":=") & visit(node.value) + Doc::text(";");
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::IfStatement &node) const -> Doc
{
    // TODO(vedivad): Implement if statement printing
    return Doc::text("-- if statement");
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::CaseStatement &node) const -> Doc
{
    // TODO(vedivad): Implement case statement printing
    return Doc::text("-- case statement");
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::Process &node) const -> Doc
{
    // TODO(vedivad): Implement process printing
    return Doc::text("-- process");
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::ForLoop &node) const -> Doc
{
    // TODO(vedivad): Implement for loop printing
    return Doc::text("-- for loop");
}

auto PrettyPrinter::operator()([[maybe_unused]] const ast::WhileLoop &node) const -> Doc
{
    // TODO(vedivad): Implement while loop printing
    return Doc::text("-- while loop");
}

} // namespace emit
