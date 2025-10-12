#include "builder/trivia/trivia_binder.hpp"

#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "vhdlLexer.h"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <vector>

namespace builder {

void TriviaBinder::collectLeading(ast::Node::NodeComments &dst, std::size_t start_idx)
{
    const auto &toks = tokens.getTokens();

    struct Tmp
    {
        bool nl;
        const antlr4::Token *tok;
        std::size_t breaks;
    };
    std::vector<Tmp> rev;
    std::size_t budget = 0;

    for (const std::size_t i :
         std::views::iota(std::size_t{ 0 }, start_idx) | std::views::reverse) {
        const auto *t = toks[i];
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
            if ((budget != 0) && !rev.empty()) {
                rev.push_back(Tmp{ .nl = true, .tok = nullptr, .breaks = budget });
            }
            budget = 0;
            rev.push_back(Tmp{ .nl = false, .tok = t, .breaks = 0 });
            continue;
        }
    }

    std::ranges::reverse(rev);
    for (const auto &e : rev) {
        if (e.nl) {
            newlines.push(dst, /*to_leading=*/true, e.breaks);
        } else {
            comments.push(dst, /*to_leading=*/true, e.tok);
        }
    }
}

void TriviaBinder::collectTrailing(ast::Node::NodeComments &dst, StopInfo stop)
{
    const auto &toks = tokens.getTokens();
    const std::size_t n = toks.size();

    std::size_t i = stop.idx + 1;

    for (; i < n; ++i) {
        const auto *t = toks[i];
        if (t == nullptr) {
            break;
        }
        if (isNewline(t)) {
            break;
        }
        if (isComment(t) && t->getLine() == stop.line) {
            comments.push(dst, false, t);
            continue;
        }
        if (t->getChannel() != vhdlLexer::COMMENTS) {
            continue;
        }
        break;
    }

    std::size_t breaks = 0;
    for (; i < n; ++i) {
        const auto *t = toks[i];
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
                         .line = static_cast<int>(ctx->getStop()->getLine()) };

    collectLeading(cm, start_idx);
    collectTrailing(cm, stop);
}

} // namespace builder
