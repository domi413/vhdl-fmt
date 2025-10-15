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

void TriviaBinder::collectTrailing(ast::Node::NodeComments &dst, const AnchorToken &anchor)
{
    const auto &tokens = tokens_.getTokens();

    // Collect trailing comments that appear on the same line as the anchor
    const auto next_tokens = tokens | std::views::drop(anchor.index + 1);

    for (const antlr4::Token *token : next_tokens) {
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
    const AnchorToken stop{ .index = ctx->getStop()->getTokenIndex(),
                            .line = ctx->getStop()->getLine() };

    collectLeading(comment, start_index);
    collectTrailing(comment, stop);
}

} // namespace builder
