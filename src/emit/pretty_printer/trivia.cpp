#include "ast/node.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <algorithm>
#include <functional>
#include <optional>
#include <ranges>
#include <variant>

namespace emit {

namespace {
auto printTrivia(const ast::Trivia &trivia) noexcept -> Doc
{
    if (const auto *comment = std::get_if<ast::Comments>(&trivia)) {
        return Doc::text(comment->text) + Doc::hardline();
    }

    if (const auto *para = std::get_if<ast::ParagraphBreak>(&trivia)) {
        Doc result = Doc::empty();

        for (unsigned int i = 0; i < para->blank_lines; ++i) {
            result += Doc::hardline();
        }

        return result;
    }

    return Doc::empty();
}
} // namespace

/**
 * @brief This is the main wrapper. It combines the core doc with trivia.
 */
auto PrettyPrinter::withTrivia(const ast::NodeBase &node, Doc core_doc) -> Doc
{
    if (!node.trivia) {
        return core_doc; // No trivia, return as-is
    }

    const Doc head = node.trivia
                       .transform([](const ast::NodeTrivia &t) -> emit::Doc {
                           auto docs_view = t.leading | std::views::transform(printTrivia);
                           return std::ranges::fold_left(docs_view, Doc::empty(), std::plus<Doc>{});
                       })
                       .value_or(Doc::empty());

    const Doc foot = node.trivia->trailing
                       .transform([](const ast::Comments &c) -> Doc {
                           return Doc::text(c.text) + Doc::noGroupMark();
                       })
                       .value_or(Doc::empty());

    if (foot.isEmpty()) {
        return head + core_doc;
    }

    return head + core_doc & foot;
}

} // namespace emit
