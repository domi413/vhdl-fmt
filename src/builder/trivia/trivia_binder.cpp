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
    const auto &toks = tokens.getTokens();

    struct Tmp
    {
        bool nl{};
        const antlr4::Token *tok{};
        std::size_t breaks{};
    };

    std::vector<Tmp> rev;
    std::size_t budget = 0;

    // Iterate backward from start_idx, collecting comment and newline tokens.
    for (const auto i : std::views::iota(std::size_t{ 0 }, start_idx) | std::views::reverse) {

        const antlr4::Token *const t = toks[i];
        if (t == nullptr) {
            break;
        }

        const auto ch = t->getChannel();
        if (ch != vhdlLexer::COMMENTS && ch != vhdlLexer::NEWLINES) {
            break;
        }

        if (isNewline(t)) {
            budget += countLineBreaks(t->getText());
            if (budget >= 2 && rev.empty()) {
                break; // blank line boundary
            }
            continue;
        }

        if (isComment(t)) {
            if (budget != 0 && !rev.empty()) {
                rev.push_back({ .nl = true, .tok = nullptr, .breaks = budget });
            }

            budget = 0;
            rev.push_back({ .nl = false, .tok = t });
        }
    }

    // Push trivia items in forward order
    for (const auto &e : rev | std::views::reverse) {
        if (e.nl) {
            newlines.push(dst, /*to_leading=*/true, e.breaks);
        } else {
            comments.push(dst, /*to_leading=*/true, e.tok);
        }
    }
}

void TriviaBinder::collectTrailing(ast::Node::NodeComments &dst, const StopInfo &stop)
{
    const auto &toks = tokens.getTokens();
    const std::size_t n = toks.size();

    std::size_t i = stop.idx + 1;

    // Collect trailing comments that appear on the same line as the stop token
    for (; i < n; ++i) {
        const antlr4::Token *const t = toks[i];
        if (t == nullptr) {
            break;
        }
        if (isNewline(t)) {
            break;
        }
        if (isComment(t) && static_cast<std::size_t>(t->getLine()) == stop.line) {
            comments.push(dst, /*to_leading=*/false, t);
            continue;
        }
        if (t->getChannel() != vhdlLexer::COMMENTS) {
            continue;
        }
        break;
    }

    // Collect subsequent newline tokens
    std::size_t breaks = 0;
    for (; i < n; ++i) {
        const antlr4::Token *const t = toks[i];
        if ((t == nullptr) || !isNewline(t)) {
            break;
        }
        breaks += countLineBreaks(t->getText());
    }

    newlines.push(dst, /*to_leading=*/false, breaks);
}

void TriviaBinder::bind(ast::Node &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }

    auto &cm = node.getComments();
    const auto start_idx = static_cast<std::size_t>(ctx->getStart()->getTokenIndex());
    const StopInfo stop{ .idx = static_cast<std::size_t>(ctx->getStop()->getTokenIndex()),
                         .line = static_cast<std::size_t>(ctx->getStop()->getLine()) };

    collectLeading(cm, start_idx);
    collectTrailing(cm, stop);
}

} // namespace builder
