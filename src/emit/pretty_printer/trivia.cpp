#include "ast/node.hpp"
#include "common/overload.hpp"
#include "emit/pretty_printer.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <algorithm>
#include <ranges>
#include <span>
#include <variant>

namespace emit {

namespace {

constexpr auto printLines(const unsigned count) -> Doc
{
    auto hardlines = std::views::repeat(Doc::hardline(), count);
    return std::ranges::fold_left(hardlines, Doc::empty(), std::plus<>());
}

constexpr auto printTrivia(const ast::Trivia &trivia) -> Doc
{
    return std::visit(
      common::Overload{
        [](const ast::Comment &c) -> Doc { return Doc::text(c.text) + Doc::hardline(); },
        [](const ast::ParagraphBreak &p) -> Doc { return printLines(p.blank_lines); } },
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

    // Print last trivia with special rules
    result += std::visit(
      common::Overload{
        [](const ast::Comment &c) -> Doc { return Doc::text(c.text) + Doc::noGroup(); },
        [](const ast::ParagraphBreak &p) -> Doc { return printLines(p.blank_lines - 1); } },
      list.back());

    return result;
}

} // namespace

auto PrettyPrinter::withTrivia(const ast::NodeBase &node, Doc core_doc) -> Doc
{
    if (!node.trivia) {
        return core_doc;
    }

    const auto &trivia = *node.trivia;

    Doc result = std::ranges::fold_left(
      trivia.leading | std::views::transform(printTrivia), Doc::empty(), std::plus<>());

    result += core_doc;

    result += trivia.inline_comment
              ? Doc::text(" ") + Doc::text(trivia.inline_comment->text) + Doc::noGroup()
              : Doc::empty();

    result += printTrailingTriviaList(trivia.trailing);

    return result;
}

} // namespace emit
