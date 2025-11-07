#include "ast/nodes/design_units.hpp"

#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

namespace emit {

auto PrettyPrinter::operator()(const ast::Entity &node) -> Doc
{
    // entity <name> is
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
    if (!node.decls.empty()) {
        for (const auto &decl : node.decls) {
            result = result << visit(decl);
        }
    }

    // Begin section (concurrent statements)
    if (!node.stmts.empty()) {
        result = result / Doc::text("begin");
        for (const auto &stmt : node.stmts) {
            result = result << visit(stmt);
        }
    }

    // end [entity] [<name>];
    Doc end_line = Doc::text("end");
    if (node.end_label.has_value()) {
        end_line = end_line & Doc::text("entity") & Doc::text(node.end_label.value());
    } else {
        end_line = end_line & Doc::text("entity") & Doc::text(node.name);
    }
    end_line = end_line + Doc::text(";");

    result = result / end_line;

    return result;
}

auto PrettyPrinter::operator()(const ast::Architecture &node) -> Doc
{
    // architecture <name> of <entity_name> is
    Doc result = Doc::text("architecture")
               & Doc::text(node.name)
               & Doc::text("of")
               & Doc::text(node.entity_name)
               & Doc::text("is");

    // Declarations
    if (!node.decls.empty()) {
        for (const auto &decl : node.decls) {
            result = result / visit(decl).nest(2);
        }
    }

    // begin
    result = result / Doc::text("begin");

    // Concurrent statements
    if (!node.stmts.empty()) {
        for (const auto &stmt : node.stmts) {
            result = result / visit(stmt).nest(2);
        }
    }

    // end [architecture] [<name>];
    const Doc end_line
      = Doc::text("end") & Doc::text("architecture") & Doc::text(node.name) + Doc::text(";");
    result = result / end_line;

    return result;
}

} // namespace emit
