#include "builder/trivia/trivia_binder.hpp"

#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>

namespace builder {

void TriviaBinder::collectLeading(ast::NodeTrivia &dst, std::size_t start_index)
{
    // Tokens are given in source order
    const auto &hidden = tokens_.getHiddenTokensToLeft(start_index);

    std::size_t linebreaks{ 0 };

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
                newlines_.push(dst, linebreaks);
                linebreaks = 0;
            }

            comments_.push(dst, /*to_leading=*/true, token);
        }
    }

    // Push any remaining paragraph breaks (2+ newlines = 1+ blank lines)
    // We only capture these to preserve intentional grouping, not every single newline
    if (linebreaks >= 2) {
        newlines_.push(dst, linebreaks);
    }

    // Iteration went backward - restore to source order
    std::ranges::reverse(dst.leading);
}

void TriviaBinder::collectTrailing(ast::NodeTrivia &dst, const AnchorToken &anchor)
{
    // Collect trailing comments that appear on the same line as the anchor
    const auto hidden = tokens_.getHiddenTokensToRight(anchor.index);

    for (const antlr4::Token *token : hidden) {
        if (token == nullptr || isNewline(token)) {
            break;
        }

        if (isComment(token) && token->getLine() == anchor.line) {
            comments_.push(dst, /*to_leading=*/false, token);
        }
    }
}

void TriviaBinder::bind(ast::NodeBase &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }

    auto &trivia = node.trivia.emplace();

    const auto start_index = ctx->getStart()->getTokenIndex();
    const AnchorToken stop{ .index = findLastDefaultOnLine(ctx->getStop()->getTokenIndex()),
                            .line = ctx->getStop()->getLine() };

    collectLeading(trivia, start_index);
    collectTrailing(trivia, stop);
}

auto TriviaBinder::findLastDefaultOnLine(std::size_t start_index) const noexcept -> std::size_t
{
    const auto *start_token = tokens_.get(start_index);
    if (start_token == nullptr) {
        return start_index;
    }

    const auto line = start_token->getLine();
    auto default_tokens_on_line = std::views::iota(start_index + 1)
                                | std::views::take_while([this, line](const std::size_t i) -> bool {
                                      const auto *token = tokens_.get(i);
                                      return (token != nullptr) && (token->getLine() == line);
                                  })
                                | std::views::filter([this](const std::size_t i) -> bool {
                                      return isDefault(tokens_.get(i));
                                  });

    std::size_t last_default = start_index;
    for (const auto i : default_tokens_on_line) {
        last_default = i;
    }

    return last_default;
}

} // namespace builder
