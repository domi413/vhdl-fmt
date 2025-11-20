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

    // Process all items except the last one normally
    const Doc result = std::ranges::fold_left(
      list | std::views::take(list.size() - 1) | std::views::transform(printTrivia),
      Doc::hardline(),
      std::plus<>());

    auto com = [](const ast::Comment &c) -> Doc { return Doc::text(c.text); };

    // Special handling for the very last trailing item
    auto par = [](const ast::ParagraphBreak &p) -> Doc {
        return Doc::hardlines((p.blank_lines > 0) ? p.blank_lines - 1 : 0);
    };

    return result + std::visit(common::Overload{ com, par }, list.back());
}

} // namespace

auto PrettyPrinter::withTrivia(const ast::NodeBase &node, Doc core_doc) -> Doc
{
    if (!node.hasTrivia()) {
        return core_doc;
    }

    const Doc leading = std::ranges::fold_left(
      node.getLeading() | std::views::transform(printTrivia), Doc::empty(), std::plus<>());

    const auto inline_text = node.getInlineComment();
    const Doc inline_comment
      = inline_text ? Doc::text(" ") + Doc::text(*inline_text) + Doc::hardlines(0) : Doc::empty();

    const Doc trailing = printTrailingTriviaList(node.getTrailing());

    return leading + core_doc + inline_comment + trailing;
}

} // namespace emit
