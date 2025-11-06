#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

// Recursion is used for traversing expression trees, which are typically shallow in VHDL parsing.
// NOLINTBEGIN(misc-no-recursion)

namespace builder {

auto Translator::makeExpr(vhdlParser::ExpressionContext *ctx) -> ast::Expr
{
    const auto &relations = ctx->relation();
    if (relations.size() == 1) {
        return makeRelation(relations[0]);
    }
    return makeBinary(ctx,
                      ctx->logical_operator(0)->getText(),
                      makeRelation(relations[0]),
                      makeRelation(relations[1]));
}

auto Translator::makeRelation(vhdlParser::RelationContext *ctx) -> ast::Expr
{
    auto *rel_op = ctx->relational_operator();
    if (rel_op == nullptr) {
        return makeShiftExpr(ctx->shift_expression(0));
    }
    const auto &shift_exprs = ctx->shift_expression();
    return makeBinary(
      ctx, rel_op->getText(), makeShiftExpr(shift_exprs[0]), makeShiftExpr(shift_exprs[1]));
}

auto Translator::makeShiftExpr(vhdlParser::Shift_expressionContext *ctx) -> ast::Expr
{
    auto *shift_op = ctx->shift_operator();
    if (shift_op == nullptr) {
        return makeSimpleExpr(ctx->simple_expression(0));
    }
    const auto &simple_exprs = ctx->simple_expression();
    return makeBinary(
      ctx, shift_op->getText(), makeSimpleExpr(simple_exprs[0]), makeSimpleExpr(simple_exprs[1]));
}

auto Translator::makeSimpleExpr(vhdlParser::Simple_expressionContext *ctx) -> ast::Expr
{
    if (ctx->PLUS() != nullptr || ctx->MINUS() != nullptr) {
        return makeUnary(ctx, ctx->PLUS() != nullptr ? "+" : "-", makeTerm(ctx->term(0)));
    }
    const auto &adding_ops = ctx->adding_operator();
    if (adding_ops.empty()) {
        return makeTerm(ctx->term(0));
    }
    const auto &terms = ctx->term();
    return makeBinary(ctx, adding_ops[0]->getText(), makeTerm(terms[0]), makeTerm(terms[1]));
}

auto Translator::makeTerm(vhdlParser::TermContext *ctx) -> ast::Expr
{
    const auto &mult_ops = ctx->multiplying_operator();
    if (mult_ops.empty()) {
        return makeFactor(ctx->factor(0));
    }
    const auto &factors = ctx->factor();
    return makeBinary(ctx, mult_ops[0]->getText(), makeFactor(factors[0]), makeFactor(factors[1]));
}

auto Translator::makeFactor(vhdlParser::FactorContext *ctx) -> ast::Expr
{
    const auto &primaries = ctx->primary();
    if (ctx->DOUBLESTAR() != nullptr) {
        return makeBinary(ctx, "**", makePrimary(primaries[0]), makePrimary(primaries[1]));
    }
    if (ctx->ABS() != nullptr) {
        return makeUnary(ctx, "abs", makePrimary(primaries[0]));
    }
    if (ctx->NOT() != nullptr) {
        return makeUnary(ctx, "not", makePrimary(primaries[0]));
    }
    return makePrimary(primaries[0]);
}

auto Translator::makePrimary(vhdlParser::PrimaryContext *ctx) -> ast::Expr
{
    if (auto *expr = ctx->expression()) {
        auto paren = makeLight<ast::ParenExpr>();
        trivia_.bind(paren, ctx);
        paren.inner = box(makeExpr(expr));
        return paren;
    }
    if (auto *agg = ctx->aggregate()) {
        return makeAggregate(agg);
    }
    if (auto *name_ctx = ctx->name()) {
        return makeName(name_ctx);
    }
    return makeToken(ctx, ctx->getText());
}

} // namespace builder

// NOLINTEND(misc-no-recursion)
