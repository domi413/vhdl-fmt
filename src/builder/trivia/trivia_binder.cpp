#include "builder/trivia/trivia_binder.hpp"

#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>

namespace builder {

void TriviaBinder::collectLeading(ast::NodeTrivia &dst, std::size_t index)
{
    // Tokens are given in source order
    const auto &hidden = tokens_.getHiddenTokensToLeft(index);
    unsigned int linebreaks{ 0 };

    // Iterate backward — closest token first
    for (const antlr4::Token *token : hidden | std::views::reverse) {
        if (token == nullptr) {
            break;
        }

        if (isNewline(token)) {
            ++linebreaks;
            continue;
        }

        if (isComment(token)) {
            // If there were newlines before this comment, push them
            if (linebreaks != 0) {
                newlines_.push(dst.leading, linebreaks);
                linebreaks = 0;
            }

            comments_.push(dst.leading, token);
        }
    }

    // Push any remaining paragraph breaks (2+ newlines = 1+ blank lines)
    // We only capture these to preserve intentional grouping, not every single newline
    if (linebreaks >= 2) {
        newlines_.push(dst.leading, linebreaks);
    }

    // Iteration went backward - restore to source order
    std::ranges::reverse(dst.leading);
}

void TriviaBinder::collectTrailing(ast::NodeTrivia &dst, std::size_t index)
{
    const auto hidden = tokens_.getHiddenTokensToRight(index);
    unsigned int linebreaks{ 0 };

    for (const antlr4::Token *token : hidden) {
        if (token == nullptr) {
            break;
        }

        // Stop if we hit a token that belongs to the next node
        // (This is where your "cache" idea comes in)
        if (comments_.wasTokenUsed(token->getTokenIndex())) {
            break;
        }

        if (isNewline(token)) {
            ++linebreaks;
            continue;
        }

        if (isComment(token)) {
            if (linebreaks >= 2) {
                newlines_.push(dst.trailing, linebreaks);
            }
            linebreaks = 0;
            comments_.push(dst.trailing, token);
        }
    }
}

auto TriviaBinder::collectInline(ast::NodeTrivia &dst, std::size_t index) -> const antlr4::Token *
{
    const auto hidden = tokens_.getHiddenTokensToRight(index);

    const antlr4::Token *result{};

    for (const antlr4::Token *token : hidden) {
        if (token == nullptr) {
            break;
        }

        if (isComment(token)) {
            result = token;
            comments_.push(dst.inline_comment, token);
            break; // only one inline comment per node
        }

        if (isNewline(token)) {
            break; // no inline comment found
        }
    }

    return result;
}

void TriviaBinder::bind(ast::NodeBase &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }

    auto &trivia = node.trivia.emplace();

    const auto start_index = ctx->getStart()->getTokenIndex();
    const auto stop_index = findLastDefaultOnLine(ctx->getStop()->getTokenIndex());

    collectLeading(trivia, start_index);
    const auto *inline_comment_token = collectInline(trivia, stop_index);

    if (inline_comment_token == nullptr) {
        collectTrailing(trivia, stop_index);
    } else {
        collectTrailing(trivia, inline_comment_token->getTokenIndex());
    }
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
