#include "ast/node.hpp"
#include "common/overload.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <ranges>
#include <variant>
#include <vector>

namespace emit {

namespace {

auto printTrivia(const ast::Trivia &trivia) -> Doc
{
    auto comment_func
      = [](const ast::Comment &c) -> Doc { return Doc::text(c.text) + Doc::hardline(); };

    auto paragraph_func = [](const ast::ParagraphBreak &p) -> Doc {
        Doc result = Doc::empty();
        for (unsigned i = 0; i < p.blank_lines; ++i) {
            result += Doc::hardline();
        }
        return result;
    };

    return std::visit(common::Overload{ comment_func, paragraph_func }, trivia);
}

auto printTrailingTriviaList(const std::vector<ast::Trivia> &list) -> Doc
{
    if (list.empty()) {
        return Doc::empty();
    }

    // Process all but the last trivia using the standard printer
    Doc result = Doc::empty();
    for (const auto &trivia : list | std::views::take(list.size() - 1)) {
        result += printTrivia(trivia);
    }

    // Append last trivia: if it's a comment, omit the trailing hardline
    result += std::visit(
      common::Overload{ [](const ast::Comment &c) -> Doc { return Doc::text(c.text); },
                        [](const auto &other) { return printTrivia(other); } },
      list.back());

    return result;
}

} // namespace

/**
 * @brief Combines the core doc with leading, inline, and trailing trivia.
 */
auto PrettyPrinter::withTrivia(const ast::NodeBase &node, Doc core_doc) -> Doc
{
    if (!node.trivia) {
        return core_doc;
    }

    const auto &trivia = *node.trivia;

    // Build the on-line part (head + core + inline comment)
    Doc result = Doc::empty();
    for (const auto &t : trivia.leading) {
        result += printTrivia(t);
    }
    result += core_doc;

    if (trivia.inline_comment) {
        result += Doc::text(" ") + Doc::text(trivia.inline_comment->text) + Doc::noGroupMark();
    }

    // Append trailing trivia
    const Doc foot = printTrailingTriviaList(trivia.trailing);

    if (!foot.isEmpty()) {
        return result.isEmpty() ? foot : (result / foot);
    }

    return result;
}

} // namespace emit
