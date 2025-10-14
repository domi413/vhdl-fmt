#include "builder/trivia/trivia_binder.hpp"

#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "vhdlLexer.h"

#include <cstddef>
#include <ranges>
#include <vector>

namespace builder {

void TriviaBinder::collectLeading(ast::Node::NodeComments &dst, std::size_t start_idx)
{
    constexpr std::size_t BLANK_LINE_BOUNDARY{ 2 };

    const auto &tokens = tokens_.getTokens();

    struct Tmp
    {
        bool nl{};
        const antlr4::Token *tok{};
        std::size_t breaks{};
    };

    std::vector<Tmp> reverse;
    std::size_t new_line_count = 0;

    // Iterate backward from start_idx, collecting comment and newline tokens.
    for (const auto i : std::views::iota(std::size_t{ 0 }, start_idx) | std::views::reverse) {

        const antlr4::Token *const token = tokens[i];
        if (token == nullptr) {
            break;
        }

        const auto ch = token->getChannel();
        if (ch != vhdlLexer::COMMENTS && ch != vhdlLexer::NEWLINES) {
            break;
        }

        if (isNewline(token)) {
            new_line_count += countLineBreaks(token->getText());
            if (new_line_count >= BLANK_LINE_BOUNDARY && reverse.empty()) {
                break;
            }
            continue;
        }

        if (isComment(token)) {
            if (new_line_count != 0 && !reverse.empty()) {
                reverse.push_back({ .nl = true, .tok = nullptr, .breaks = new_line_count });
            }

            new_line_count = 0;
            reverse.push_back({ .nl = false, .tok = token });
        }
    }

    // Push trivia items in forward order
    for (const auto &elem : reverse | std::views::reverse) {
        if (elem.nl) {
            newlines_.push(dst, /*to_leading=*/true, elem.breaks);
        } else {
            comments_.push(dst, /*to_leading=*/true, elem.tok);
        }
    }
}

// FIXME(-):
void TriviaBinder::collectTrailing(ast::Node::NodeComments &dst, const StopInfo &stop)
{
    const auto &tokens = tokens_.getTokens();
    const std::size_t token_size = tokens.size();

    std::size_t i = stop.idx + 1;

    // Collect trailing comments that appear on the same line as the stop token
    for (; i < token_size; ++i) {
        const antlr4::Token *const token = tokens[i];
        if (token == nullptr) {
            break;
        }
        if (isNewline(token)) {
            break;
        }
        if (isComment(token) && token->getLine() == stop.line) {
            comments_.push(dst, /*to_leading=*/false, token);
            continue;
        }
        if (token->getChannel() != vhdlLexer::COMMENTS) {
            continue;
        }
        break;
    }

    // Collect subsequent newline tokens
    std::size_t breaks = 0;
    // dead code?
    // for (; i < n; ++i) {
    //     const antlr4::Token *const t = toks[i];
    //     if ((t == nullptr) || !isNewline(t)) {
    //         break;
    //     }
    //     breaks += countLineBreaks(t->getText());
    // }

    newlines_.push(dst, /*to_leading=*/false, breaks);
}

void TriviaBinder::bind(ast::Node &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }

    auto &comment = node.getComments();

    const auto start_idx = ctx->getStart()->getTokenIndex();
    const StopInfo stop{ .idx = ctx->getStop()->getTokenIndex(),
                         .line = ctx->getStop()->getLine() };

    collectLeading(comment, start_idx);
    collectTrailing(comment, stop);
}

} // namespace builder
