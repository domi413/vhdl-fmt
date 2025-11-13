#include "ast/nodes/design_units.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/doc_utils.hpp"

#include <ranges>

namespace emit {

auto PrettyPrinter::operator()(const ast::GenericClause &node) const -> Doc
{
    if (std::ranges::empty(node.generics)) {
        return Doc::empty();
    }

    const Doc opener = Doc::text("generic") & Doc::text("(");
    const Doc closer = Doc::text(");");

    const Doc result
      = joinDocsConditional(node.generics, [this](const auto &param, bool is_last) -> Doc {
            // 1. Get the core doc from the child visitor
            Doc core_doc = visit(param); //

            // 2. Append the semicolon *to the core doc* if needed
            if (!is_last) {
                core_doc += Doc::text(";");
            }

            // 3. NOW, wrap the (core + optional ';') with its trivia
            Doc full_doc = withTrivia(param, core_doc); //

            // 4. Add the newline separator *after* all trivia
            if (!is_last) {
                full_doc += Doc::line();
            }
            return full_doc;
        });

    const Doc aligned_result = Doc::align(result);

    // This withTrivia handles trivia for the *entire clause*
    return withTrivia(node, Doc::group(Doc::bracket(opener, aligned_result, closer)));
}

auto PrettyPrinter::operator()(const ast::PortClause &node) const -> Doc
{
    if (std::ranges::empty(node.ports)) {
        return Doc::empty();
    }

    const Doc opener = Doc::text("port") & Doc::text("(");
    const Doc closer = Doc::text(");");

    // Same logic for ports
    const Doc result
      = joinDocsConditional(node.ports, [this](const auto &port, bool is_last) -> Doc {
            Doc core_doc = visit(port); //

            if (!is_last) {
                core_doc += Doc::text(";");
            }

            Doc full_doc = withTrivia(port, core_doc); //

            if (!is_last) {
                full_doc += Doc::line();
            }
            return full_doc;
        });

    const Doc aligned_result = Doc::align(result);

    return withTrivia(node, Doc::group(Doc::bracket(opener, aligned_result, closer)));
}

} // namespace emit
