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

    // Early exit if no hidden tokens
    if (hidden.empty()) {
        return;
    }

    std::size_t linebreaks{ 0 };

    // Iterate backward — closest token first
    // Avoid std::views::reverse overhead by using reverse iterators directly
    for (auto it = hidden.rbegin(); it != hidden.rend(); ++it) {
        const antlr4::Token *token = *it;
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

    // Early exit if no hidden tokens
    if (hidden.empty()) {
        return;
    }

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
    const auto &tokens = tokens_.getTokens();
    const std::size_t line = tokens[start_index]->getLine();
    const std::size_t token_count = tokens.size();

    std::size_t last_default = start_index;

    // Direct iteration without views for better performance
    for (std::size_t i = start_index + 1; i < token_count; ++i) {
        const auto *token = tokens[i];
        if (token == nullptr || token->getLine() != line) {
            break;
        }

        if (isDefault(token)) {
            last_default = i;
        }
    }

    return last_default;
}

} // namespace builder
