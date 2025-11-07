#include "ast/nodes/design_units.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <algorithm>
#include <utility>

namespace emit {

namespace {
// Helper to append multiple items with << operator
auto appendAll(Doc initial, const auto &items, auto visitor) -> Doc
{
    return std::ranges::fold_left(items | std::views::transform(visitor),
                                  std::move(initial),
                                  [](const Doc &acc, const Doc &doc) -> Doc { return acc << doc; });
}
} // namespace

auto PrettyPrinter::operator()(const ast::Entity &node) -> Doc
{
    Doc result = Doc::text("entity") & Doc::text(node.name) & Doc::text("is");

    // Generic clause (if not empty)
    if (!node.generic_clause.generics.empty()) {
        result = result << visit(node.generic_clause);
    }

    // Port clause (if not empty)
    if (!node.port_clause.ports.empty()) {
        result = result << visit(node.port_clause);
    }

    // Declarations
    result = appendAll(result, node.decls, [this](const auto &decl) { return visit(decl); });

    // Begin section (concurrent statements)
    if (!node.stmts.empty()) {
        result = result / Doc::text("begin");
        result = appendAll(result, node.stmts, [this](const auto &stmt) { return visit(stmt); });
    }

    // end [entity] [<name>];
    const auto end_label = node.end_label.value_or(node.name);
    const Doc end_line
      = Doc::text("end") & Doc::text("entity") & Doc::text(end_label) + Doc::text(";");

    return result / end_line;
}

auto PrettyPrinter::operator()(const ast::Architecture &node) -> Doc
{
    Doc result = Doc::text("architecture")
               & Doc::text(node.name)
               & Doc::text("of")
               & Doc::text(node.entity_name)
               & Doc::text("is");

    // Declarations
    result = appendAll(result, node.decls, [this](const auto &decl) { return visit(decl); });

    // begin
    result = result / Doc::text("begin");

    // Concurrent statements
    result = appendAll(result, node.stmts, [this](const auto &stmt) { return visit(stmt); });

    // end [architecture] [<name>];
    const Doc end_line
      = Doc::text("end") & Doc::text("architecture") & Doc::text(node.name) + Doc::text(";");

    return result / end_line;
}

} // namespace emit
