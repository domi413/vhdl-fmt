#include "ast/node.hpp"
#include "common/overload.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/doc_utils.hpp"

#include <algorithm>
#include <functional>
#include <ranges>
#include <vector>

namespace emit {

namespace {
/**
 * @brief Handles a single `ast::Trivia` variant (for leading trivia).
 */
auto printTrivia(const ast::Trivia &trivia) -> Doc
{
    auto comments = [](const ast::Comments &comment) -> Doc {
        return Doc::text(comment.text) + Doc::hardline();
    };

    auto para_break = [](const ast::ParagraphBreak &para) -> Doc {
        Doc result = Doc::empty();
        for (unsigned int i = 0; i < para.blank_lines; ++i) {
            result += Doc::hardline();
        }
        return result;
    };

    return std::visit(common::Overload{ comments, para_break }, trivia);
}
} // namespace

/**
 * @brief This is the main wrapper. It combines the core doc with trivia.
 */
auto PrettyPrinter::withTrivia(const ast::NodeBase &node, Doc core_doc) const -> Doc
{
    if (!node.trivia) {
        return core_doc; // No trivia, return as-is
    }

    Doc leading = handleLeading(node.trivia);
    Doc trailing = handleTrailing(node.trivia);

    if (trailing.isEmpty()) {
        // No trailing comment, just prepend leading trivia
        return leading + core_doc;
    }

    // Has a trailing comment. Attach with a space.
    // e.g., "coreDoc" & "-- trailing comment"
    return leading + core_doc & trailing;
}

/**
 * @brief Handles leading trivia (comments and paragraph breaks).
 */
auto PrettyPrinter::handleLeading(const std::optional<ast::NodeTrivia> &trivia) const -> Doc
{
    if (!trivia || trivia->leading.empty()) {
        return Doc::empty();
    }

    auto trivia_docs = trivia->leading | std::views::transform([this](const auto &t) -> Doc {
                           return printTrivia(t);
                       });

    return std::ranges::fold_left(trivia_docs, Doc::empty(), std::plus<Doc>{});
}

/**
 * @brief Handles trailing trivia (which are *only* comments).
 */
auto PrettyPrinter::handleTrailing(const std::optional<ast::NodeTrivia> &trivia) const -> Doc
{
    if (!trivia || trivia->trailing.empty()) {
        return Doc::empty();
    }

    auto comment_docs
      = trivia->trailing | std::views::transform([this](const ast::Comments &comment) -> Doc {
            return Doc::text(comment.text);
        });

    std::vector<Doc> docs_vec = comment_docs | std::ranges::to<std::vector>();
    return joinDocs(docs_vec, Doc::text(" "), false);
}

} // namespace emit
