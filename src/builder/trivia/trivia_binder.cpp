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

    // Use std::variant instead of struct with bool flag
    struct Newline
    {
        std::size_t breaks;
    };

    struct Comment
    {
        const antlr4::Token *token;
    };
    using TriviaItem = std::variant<Newline, Comment>;

    std::vector<TriviaItem> reverse{};
    std::size_t newline_count{ 0 };

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
            newline_count += countLineBreaks(token->getText());
            if (newline_count >= BLANK_LINE_BOUNDARY && reverse.empty()) {
                break;
            }
            continue;
        }

        if (isComment(token)) {
            if (newline_count != 0 && !reverse.empty()) {
                reverse.emplace_back(Newline{ newline_count });
            }

            newline_count = 0;
            reverse.emplace_back(Comment{ token });
        }
    }

    // Push trivia items in forward order
    for (const auto &item : reverse | std::views::reverse) {
        if (std::holds_alternative<Newline>(item)) {
            const auto &nl = std::get<Newline>(item);
            newlines_.push(dst, /*to_leading=*/true, nl.breaks);
        } else {
            const auto &cmt = std::get<Comment>(item);
            comments_.push(dst, /*to_leading=*/true, cmt.token);
        }
    }
}

// TODO(-): Decide for one of two versions:
//  range based approach, though some overhead because of transform, but compiler optimization
//  will make this irrelevant
// void TriviaBinder::collectTrailing(ast::Node::NodeComments &dst, const StopInfo &stop)
// {
//     const auto &tokens = tokens_.getTokens();
//
//     // Collect trailing comments that appear on the same line as the stop token
//     auto trailing_comments
//       = std::views::iota(stop.index + 1, tokens.size())
//       | std::views::transform(
//           [&tokens](std::size_t i) -> const antlr4::Token * { return tokens[i]; })
//       | std::views::take_while(
//           [](const antlr4::Token *token) -> bool { return token != nullptr && !isNewline(token);
//           })
//       | std::views::filter([this, &stop](const antlr4::Token *token) -> bool {
//             return isComment(token) && token->getLine() == stop.line;
//         });
//
//     for (const antlr4::Token *token : trailing_comments) {
//         comments_.push(dst, /*to_leading=*/false, token);
//     }
//
//     newlines_.push(dst, /*to_leading=*/false, 0);
// }

// Simpler and maybe more readable approach that is in theory more performant
void TriviaBinder::collectTrailing(ast::Node::NodeComments &dst, const StopInfo &stop)
{
    const auto &tokens = tokens_.getTokens();

    // Collect trailing comments that appear on the same line as the stop token
    const auto remaining = tokens | std::views::drop(stop.index + 1);

    for (const antlr4::Token *token : remaining) {
        if (token == nullptr || isNewline(token)) {
            break;
        }

        if (isComment(token) && token->getLine() == stop.line) {
            comments_.push(dst, /*to_leading=*/false, token);
        } else if (token->getChannel() != vhdlLexer::COMMENTS) {
            break;
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
    const StopInfo stop{ .index = ctx->getStop()->getTokenIndex(),
                         .line = ctx->getStop()->getLine() };

    collectLeading(comment, start_index);
    collectTrailing(comment, stop);
}

} // namespace builder
