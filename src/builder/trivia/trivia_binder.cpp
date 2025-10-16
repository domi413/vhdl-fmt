#include "builder/trivia/trivia_binder.hpp"

#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>

namespace builder {

void TriviaBinder::collectLeading(ast::Node::NodeComments &dst, std::size_t start_index)
{
    constexpr std::size_t BLANK_LINE_BOUNDARY{ 2 };

    // Tokens are given in source order
    const auto &hidden = tokens_.getHiddenTokensToLeft(start_index);

    std::size_t linebreaks{ 0 };
    bool seen_comment = false;

    // Iterate backward — closest token first
    for (const antlr4::Token *token : hidden | std::views::reverse) {
        if (token == nullptr) {
            break;
        }

        if (isNewline(token)) {
            linebreaks += countLineBreaks(token->getText());
            // Stop if blank line was hit *before* any comment
            if (linebreaks >= BLANK_LINE_BOUNDARY && !seen_comment) {
                break;
            }
            continue;
        }

        if (isComment(token)) {
            // If there were newlines before this comment, push them
            if (linebreaks != 0) {
                newlines_.push(dst, /*to_leading=*/true, linebreaks);
                linebreaks = 0;
            }

            comments_.push(dst, /*to_leading=*/true, token);
            seen_comment = true;
        }
    }

    // Iteration went backward - restore to source order
    std::ranges::reverse(dst.leading);
}

void TriviaBinder::collectTrailing(ast::Node::NodeComments &dst, const AnchorToken &anchor)
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

    newlines_.push(dst, /*to_leading=*/false, 0);
}

void TriviaBinder::bind(ast::Node &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }

    auto &comment = node.emplaceComments();

    const auto start_index = ctx->getStart()->getTokenIndex();
    const AnchorToken stop{ .index = findLastDefaultOnLine(ctx->getStop()->getTokenIndex()),
                            .line = ctx->getStop()->getLine() };

    collectLeading(comment, start_index);
    collectTrailing(comment, stop);
}

auto TriviaBinder::findLastDefaultOnLine(std::size_t start_index) const noexcept -> std::size_t
{
    const auto &tokens = tokens_.getTokens();
    const std::size_t line = tokens[start_index]->getLine();

    std::size_t last_default = start_index;

    for (auto *token : tokens | std::views::drop(start_index + 1)) {
        if (token == nullptr || token->getLine() != line) {
            break;
        }

        if (isDefault(token)) {
            last_default = token->getTokenIndex();
        }
    }

    return last_default;
}

} // namespace builder
