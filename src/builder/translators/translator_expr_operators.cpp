#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

namespace builder {

auto Translator::makeExpression(vhdlParser::ExpressionContext *ctx) -> ast::Expression
{
    if (ctx->relation().size() == 1) {
        return makeRelation(ctx->relation(0));
    }

    return makeBinary(ctx,
                      ctx->logical_operator(0)->getText(),
                      makeRelation(ctx->relation(0)),
                      makeRelation(ctx->relation(1)));
}

auto Translator::makeRelation(vhdlParser::RelationContext *ctx) -> ast::Expression
{
    if (ctx->relational_operator() == nullptr) {
        return makeShiftExpression(ctx->shift_expression(0));
    }

    return makeBinary(ctx,
                      ctx->relational_operator()->getText(),
                      makeShiftExpression(ctx->shift_expression(0)),
                      makeShiftExpression(ctx->shift_expression(1)));
}

auto Translator::makeShiftExpression(vhdlParser::Shift_expressionContext *ctx) -> ast::Expression
{
    if (ctx->shift_operator() == nullptr) {
        return makeSimpleExpression(ctx->simple_expression(0));
    }

    return makeBinary(ctx,
                      ctx->shift_operator()->getText(),
                      makeSimpleExpression(ctx->simple_expression(0)),
                      makeSimpleExpression(ctx->simple_expression(1)));
}

auto Translator::makeSimpleExpression(vhdlParser::Simple_expressionContext *ctx) -> ast::Expression
{
    if (ctx->PLUS() != nullptr || ctx->MINUS() != nullptr) {
        return makeUnary(ctx, ctx->PLUS() != nullptr ? "+" : "-", makeTerm(ctx->term(0)));
    }

    if (ctx->adding_operator().empty()) {
        return makeTerm(ctx->term(0));
    }

    return makeBinary(
      ctx, ctx->adding_operator(0)->getText(), makeTerm(ctx->term(0)), makeTerm(ctx->term(1)));
}

auto Translator::makeTerm(vhdlParser::TermContext *ctx) -> ast::Expression
{
    if (ctx->multiplying_operator().empty()) {
        return makeFactor(ctx->factor(0));
    }

    return makeBinary(ctx,
                      ctx->multiplying_operator(0)->getText(),
                      makeFactor(ctx->factor(0)),
                      makeFactor(ctx->factor(1)));
}

auto Translator::makeFactor(vhdlParser::FactorContext *ctx) -> ast::Expression
{
    if (ctx->DOUBLESTAR() != nullptr) {
        return makeBinary(ctx, "**", makePrimary(ctx->primary(0)), makePrimary(ctx->primary(1)));
    }

    if (ctx->ABS() != nullptr) {
        return makeUnary(ctx, "abs", makePrimary(ctx->primary(0)));
    }

    if (ctx->NOT() != nullptr) {
        return makeUnary(ctx, "not", makePrimary(ctx->primary(0)));
    }

    return makePrimary(ctx->primary(0));
}

auto Translator::makePrimary(vhdlParser::PrimaryContext *ctx) -> ast::Expression
{
    if (ctx->expression() != nullptr) {
        auto paren = make<ast::ParenExpr>(ctx);
        paren.inner = box(makeExpression(ctx->expression()));
        return paren;
    }

    if (ctx->aggregate() != nullptr) {
        return makeAggregate(ctx->aggregate());
    }

    if (auto *name_ctx = ctx->name()) {
        return makeName(name_ctx);
    }

    return makeToken(ctx, ctx->getText());
}

} // namespace builder
