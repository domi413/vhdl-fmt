#include "builder/comment_binder.hpp"

#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "vhdlLexer.h"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <utility>
#include <vector>

namespace builder {

void CommentBinder::pushIfFresh(const antlr4::Token *t, std::vector<ast::Comment> &vec)
{
    if (t == nullptr) {
        return;
    }
    const auto idx = t->getTokenIndex();
    if (used.insert(idx).second) {
        vec.push_back({ t->getText(), t->getLine() });
    }
}

void CommentBinder::collectLeading(ast::Node::NodeComments &dst, std::size_t start_idx)
{
    const auto &toks = tokens.getTokens();

    std::size_t newline_budget = 0;         // line breaks since last comment
    std::vector<const antlr4::Token *> acc; // collected in reverse while walking left

    // Walk indices [0, startIdx) in reverse
    for (const std::size_t i :
         std::views::iota(std::size_t{ 0 }, start_idx) | std::views::reverse) {
        const auto *t = toks[i];
        if (t == nullptr) {
            break;
        }

        const auto ch = t->getChannel();
        if (ch != vhdlLexer::COMMENTS && ch != vhdlLexer::NEWLINES) {
            // hit default or other hidden channel => stop
            break;
        }

        if (isNewline(t)) {
            newline_budget += countLineBreaks(t->getText());
            if (newline_budget >= 2) {
                break; // blank line separates blocks
            }
            continue;
        }

        if (isComment(t)) {
            newline_budget = 0;
            acc.push_back(t);
            continue;
        }
    }

    std::ranges::reverse(acc);
    for (const auto *t : acc) {
        pushIfFresh(t, dst.leading);
    }
}

void CommentBinder::collectInline(ast::Node::NodeComments &dst, StopInfo stop)
{
    const auto &toks = tokens.getTokens();
    const std::size_t n = toks.size();

    // Walk indices (stop.idx, n) forward
    for (const std::size_t i : std::views::iota(stop.idx + 1, n)) {
        const auto *t = toks[i];
        if (t == nullptr) {
            break;
        }

        if (isNewline(t)) {
            break; // end of line
        }
        if (!isComment(t)) {
            continue; // skip other hidden tokens
        }

        if (t->getLine() == stop.line) {
            pushIfFresh(t, dst.trailing);
        } else {
            break; // comment not on same line → not inline
        }
    }
}

void CommentBinder::bind(ast::Node &node, const antlr4::ParserRuleContext *ctx)
{
    if (ctx == nullptr) {
        return;
    }

    auto &cm = node.getComments();
    const auto start_idx = ctx->getStart()->getTokenIndex();
    const StopInfo stop{ .idx = ctx->getStop()->getTokenIndex(),
                         .line = ctx->getStop()->getLine() };

    collectLeading(cm, start_idx);
    collectInline(cm, stop);
}

} // namespace builder
