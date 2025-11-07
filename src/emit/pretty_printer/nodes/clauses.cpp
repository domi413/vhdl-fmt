#include "ast/nodes/design_units.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <algorithm>
#include <ranges>

namespace emit {

auto PrettyPrinter::operator()(const ast::GenericClause &node) -> Doc
{
    if (node.generics.empty()) {
        return Doc::empty();
    }

    // Build list of generic parameters using fold_left
    auto params = std::ranges::fold_left(
      node.generics | std::views::transform([this](const auto &g) { return visit(g); }),
      Doc::empty(),
      [first = true](const Doc &acc, const Doc &doc) mutable -> Doc {
          if (first) {
              first = false;
              return doc;
          }
          return acc + Doc::text(";") / doc;
      });

    const Doc opener = Doc::text("generic") & Doc::text("(");
    const Doc closer = Doc::text(");");

    return (opener << params >> closer).group();
}

auto PrettyPrinter::operator()(const ast::PortClause &node) -> Doc
{
    if (node.ports.empty()) {
        return Doc::empty();
    }

    // Build list of ports using fold_left
    auto ports = std::ranges::fold_left(
      node.ports | std::views::transform([this](const auto &p) { return visit(p); }),
      Doc::empty(),
      [first = true](const Doc &acc, const Doc &doc) mutable -> Doc {
          if (first) {
              first = false;
              return doc;
          }
          return acc + Doc::text(";") / doc;
      });

    const Doc opener = Doc::text("port") & Doc::text("(");
    const Doc closer = Doc::text(");");

    return (opener << ports >> closer).group();
}

} // namespace emit
