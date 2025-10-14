#include "builder/trivia/trivia_binder.hpp"

#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "vhdlLexer.h"

#include <cstddef>
#include <ranges>
#include <vector>

namespace builder {

void TriviaBinder::collectLeading(ast::Node::NodeComments &dst, std::size_t start_index)
{
    constexpr std::size_t BLANK_LINE_BOUNDARY{ 2 };

    const auto &tokens = tokens_.getTokens();

    struct Tmp
    {
        bool new_line{};
        const antlr4::Token *token{};
        std::size_t breaks{};
    };

    std::vector<Tmp> reverse{};
    std::size_t new_line_count{ 0 };

    // Iterate backward from start_index, collecting comment and newline tokens.
    for (const auto i : std::views::iota(std::size_t{ 0 }, start_index) | std::views::reverse) {

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
                reverse.push_back({ .new_line = true, .token = nullptr, .breaks = new_line_count });
            }

            new_line_count = 0;
            reverse.push_back({ .new_line = false, .token = token });
        }
    }

    // Push trivia items in forward order
    for (const auto &elem : reverse | std::views::reverse) {
        if (elem.new_line) {
            newlines_.push(dst, /*to_leading=*/true, elem.breaks);
        } else {
            comments_.push(dst, /*to_leading=*/true, elem.token);
        }
    }
}

void TriviaBinder::collectTrailing(ast::Node::NodeComments &dst, const StopInfo &stop)
{
    const std::vector<antlr4::Token *> &tokens = tokens_.getTokens();
    const std::size_t tokens_size = tokens.size();

    std::size_t i = stop.index + 1;

    // Collect trailing comments that appear on the same line as the stop token
    for (; i < tokens_size; ++i) {
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
    for (; i < tokens_size; ++i) {
        const antlr4::Token *const token = tokens[i];
        if ((token == nullptr) || !isNewline(token)) {
            break;
        }
        breaks += countLineBreaks(token->getText());
    }

    newlines_.push(dst, /*to_leading=*/false, breaks);
}

void TriviaBinder::bind(ast::Node &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }

    auto &comment = node.getComments();

    const auto start_index = ctx->getStart()->getTokenIndex();
    const StopInfo stop{ .index = ctx->getStop()->getTokenIndex(),
                         .line = ctx->getStop()->getLine() };

    collectLeading(comment, start_index);
    collectTrailing(comment, stop);
}

} // namespace builder
