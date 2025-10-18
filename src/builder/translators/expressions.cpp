#include "ast/nodes/expressions.hpp"

#include "ParserRuleContext.h"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <ranges>

namespace builder {

auto Translator::makeExpr(vhdlParser::ExpressionContext *ctx) -> ast::Expr &
{
    return makeTokenizedExpr(ctx);
}

auto Translator::makeSimpleExpr(vhdlParser::Simple_expressionContext *ctx) -> ast::Expr &
{
    return makeTokenizedExpr(ctx);
}

auto Translator::makeTokenizedExpr(antlr4::ParserRuleContext *ctx) -> ast::Expr &
{
    const auto *first = ctx->getStart();
    const auto *last = ctx->getStop();
    if ((first == nullptr) || (last == nullptr)) {
        auto &tok = spawn<ast::TokenExpr>(ctx);
        tok.text = ctx->getText();
        return tok;
    }

    const auto start = first->getTokenIndex();
    const auto end = last->getTokenIndex();
    if (start == end) {
        auto &tok = spawn<ast::TokenExpr>(ctx);
        tok.text = first->getText();
        return tok;
    }

    auto &group = spawn<ast::GroupExpr>(ctx);
    into(group.children, [&]() {
        for (auto index : std::views::iota(start, end + 1)) {
            auto *token = tokens_.get(index);
            if (token->getChannel() != antlr4::Token::DEFAULT_CHANNEL) {
                continue;
            }
            auto &tok = spawn<ast::TokenExpr>();
            tok.text = token->getText();
        }
    });

    return group;
}

} // namespace builder