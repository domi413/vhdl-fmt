#include "ast/node.hpp"
#include "common/overload.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <algorithm>
#include <functional>
#include <ranges>
#include <span>
#include <variant>

namespace emit {

namespace {
constexpr auto printTrivia(const ast::Trivia &trivia) -> Doc
{
    return std::visit(
      common::Overload{
        [](const ast::Comment &c) -> Doc { return Doc::text(c.text) + Doc::hardline(); },
        [](const ast::ParagraphBreak &p) -> Doc { return Doc::hardlines(p.blank_lines); } },
      trivia);
}

auto printTrailingTriviaList(const std::span<const ast::Trivia> list) -> Doc
{
    if (list.empty()) {
        return Doc::empty();
    }

    Doc result = std::ranges::fold_left(
      list | std::views::take(list.size() - 1) | std::views::transform(printTrivia),
      Doc::line(),
      std::plus<>());

    auto cm = [](const ast::Comment &c) -> Doc {
        // `hardlines(0)` makes it so it can't be flattened
        return Doc::text(c.text) + Doc::hardlines(0);
    };
    auto pb = [](const ast::ParagraphBreak &p) -> Doc {
        return Doc::hardlines(std::max(p.blank_lines - 1, 0U));
    };

    return result + std::visit(common::Overload{ cm, pb }, list.back());
}

} // namespace

auto PrettyPrinter::withTrivia(const ast::NodeBase &node, Doc core_doc) -> Doc
{
    if (!node.trivia) {
        return core_doc;
    }

    const auto &trivia = *node.trivia;

    const Doc leading = std::ranges::fold_left(
      trivia.leading | std::views::transform(printTrivia), Doc::empty(), std::plus<>());

    const Doc inline_comment
      = trivia.inline_comment
        ? Doc::text(" ") + Doc::text(trivia.inline_comment->text) + Doc::hardlines(0)
        : Doc::empty();

    const Doc trailing = printTrailingTriviaList(trivia.trailing);

    return leading + core_doc + inline_comment + trailing;
}

} // namespace emit
