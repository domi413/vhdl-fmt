#include "builder/trivia/trivia_binder.hpp"

#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "builder/trivia/utils.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>

namespace builder {

void TriviaBinder::collect(std::vector<ast::Trivia> &dst,
                           const std::vector<antlr4::Token *> &tokens)
{
    unsigned int linebreaks{ 0 };

    for (const antlr4::Token *token : tokens) {
        const auto index = token->getTokenIndex();

        if (used_.contains(index)) {
            continue;
        }

        used_.insert(index);

        if (isNewline(token)) {
            ++linebreaks;
            continue;
        }

        if (isComment(token)) {
            if (linebreaks >= 2) {
                dst.emplace_back(ast::ParagraphBreak{ .blank_lines = linebreaks - 1 });
            }
            linebreaks = 0; // reset linebreak count

            dst.emplace_back(ast::Comment{ token->getText() });
        }
    }

    if (linebreaks >= 2) {
        dst.emplace_back(ast::ParagraphBreak{ .blank_lines = linebreaks - 1 });
    }
}

auto TriviaBinder::collectInline(std::optional<ast::Comment> &dst, std::size_t index)
{
    const auto &token = tokens_.get(index);

    if (used_.contains(index)) {
        return;
    }

    if (isComment(token)) {
        used_.insert(index);
        dst.emplace(ast::Comment{ token->getText() });
        return; // only one inline comment per node
    }

    if (isNewline(token)) {
        return; // no inline comment found
    }
}

void TriviaBinder::bind(ast::NodeBase &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }

    auto &trivia = node.trivia.emplace();

    const auto start_index = ctx->getStart()->getTokenIndex();
    const auto stop_index = findLastDefaultOnLine(ctx->getStop()->getTokenIndex());

    const auto hidden_left = tokens_.getHiddenTokensToLeft(start_index);
    const auto hidden_right = tokens_.getHiddenTokensToRight(stop_index);

    collect(trivia.leading, hidden_left);
    collectInline(trivia.inline_comment, stop_index + 1);
    collect(trivia.trailing, hidden_right);
}

auto TriviaBinder::findLastDefaultOnLine(std::size_t start_index) const -> std::size_t
{
    const auto *start_token = tokens_.get(start_index);
    if (start_token == nullptr) {
        return start_index;
    }

    const auto line = start_token->getLine();

    return std::ranges::fold_left(
      std::views::iota(start_index + 1)
        | std::views::take_while([this, line](const std::size_t i) -> bool {
              const auto *token = tokens_.get(i);
              return (token != nullptr) && (token->getLine() == line);
          })
        | std::views::filter(
          [this](const std::size_t i) -> bool { return isDefault(tokens_.get(i)); }),
      start_index,
      [](std::size_t /* unused */, const std::size_t current) -> std::size_t { return current; });
}

} // namespace builder
