#include "ast/nodes/design_units.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

namespace emit {

auto PrettyPrinter::operator()(const ast::GenericClause &node) -> Doc
{
    if (node.generics.empty()) {
        return Doc::empty();
    }

    Doc result = Doc::empty();

    // generic (
    const Doc opener = Doc::text("generic") & Doc::text("(");

    // Build list of generic parameters
    Doc params = Doc::empty();
    bool first = true;

    for (const auto &generic : node.generics) {
        if (!first) {
            params = params + Doc::text(";") + Doc::line();
        }
        first = false;
        params = params + visit(generic);
    }

    const Doc closer = Doc::text(");");
    result = opener << params >> closer;

    return result.group();
}

auto PrettyPrinter::operator()(const ast::PortClause &node) -> Doc
{
    if (node.ports.empty()) {
        return Doc::empty();
    }

    Doc result = Doc::empty();

    // port (
    const Doc opener = Doc::text("port") & Doc::text("(");

    // Build list of ports
    Doc ports = Doc::empty();
    bool first = true;

    for (const auto &port : node.ports) {
        if (!first) {
            ports = ports + Doc::text(";") + Doc::line();
        }
        first = false;
        ports = ports + visit(port);
    }

    const Doc closer = Doc::text(");");
    result = opener << ports >> closer;

    return result.group();
}

} // namespace emit
