#include "ast/nodes/expressions.hpp"

#include "builder/translator.hpp"
#include "vhdlParser.h"

namespace builder {

auto Translator::makeExpr(vhdlParser::ExpressionContext *ctx) -> ast::Expr &
{
    // Single relation — descend directly.
    if (ctx->relation().size() == 1) {
        return makeRelation(ctx->relation(0));
    }

    // Otherwise: fold left-associatively
    auto &lhs = makeRelation(ctx->relation(0));
    for (size_t i = 0; i < ctx->logical_operator().size(); ++i) {
        auto &bin = spawn<ast::BinaryExpr>(ctx);
        bin.op = ctx->logical_operator(i)->getText();
        into(bin.left, [&] { dispatch_(ctx->relation(i)); });
        into(bin.right, [&] { dispatch_(ctx->relation(i + 1)); });
        return bin; // left-assoc -> outermost node
    }

    return lhs;
}

auto Translator::makeRelation(vhdlParser::RelationContext *ctx) -> ast::Expr &
{
    if (ctx->relational_operator() == nullptr) {
        return makeShiftExpr(ctx->shift_expression(0));
    }

    auto &bin = spawn<ast::BinaryExpr>(ctx);
    bin.op = ctx->relational_operator()->getText();
    into(bin.left, [&] { dispatch_(ctx->shift_expression(0)); });
    into(bin.right, [&] { dispatch_(ctx->shift_expression(1)); });
    return bin;
}

auto Translator::makeShiftExpr(vhdlParser::Shift_expressionContext *ctx) -> ast::Expr &
{
    if (ctx->shift_operator() == nullptr) {
        return makeSimpleExpr(ctx->simple_expression(0));
    }

    auto &bin = spawn<ast::BinaryExpr>(ctx);
    bin.op = ctx->shift_operator()->getText();
    into(bin.left, [&] { dispatch_(ctx->simple_expression(0)); });
    into(bin.right, [&] { dispatch_(ctx->simple_expression(1)); });
    return bin;
}

auto Translator::makeSimpleExpr(vhdlParser::Simple_expressionContext *ctx) -> ast::Expr &
{
    // unary +/- at start
    if ((ctx->PLUS() != nullptr) || (ctx->MINUS() != nullptr)) {
        auto &un = spawn<ast::UnaryExpr>(ctx);
        un.op = (ctx->PLUS() != nullptr) ? "+" : "-";
        into(un.value, [&] { dispatch_(ctx->term(0)); });
        return un;
    }

    // first term
    if (ctx->adding_operator().empty()) {
        return makeTerm(ctx->term(0));
    }

    // left-assoc fold
    auto &bin = spawn<ast::BinaryExpr>(ctx);
    bin.op = ctx->adding_operator(0)->getText();
    into(bin.left, [&] { dispatch_(ctx->term(0)); });
    into(bin.right, [&] { dispatch_(ctx->term(1)); });
    return bin;
}

auto Translator::makeTerm(vhdlParser::TermContext *ctx) -> ast::Expr &
{
    if (ctx->multiplying_operator().empty()) {
        return makeFactor(ctx->factor(0));
    }

    auto &bin = spawn<ast::BinaryExpr>(ctx);
    bin.op = ctx->multiplying_operator(0)->getText();
    into(bin.left, [&] { dispatch_(ctx->factor(0)); });
    into(bin.right, [&] { dispatch_(ctx->factor(1)); });
    return bin;
}

auto Translator::makeFactor(vhdlParser::FactorContext *ctx) -> ast::Expr &
{
    // Power operator: a ** b
    if (ctx->DOUBLESTAR() != nullptr) {
        auto &bin = spawn<ast::BinaryExpr>(ctx);
        bin.op = "**";
        into(bin.left, [&] { dispatch_(ctx->primary(0)); });
        into(bin.right, [&] { dispatch_(ctx->primary(1)); });
        return bin;
    }

    // Unary: abs or not
    if (ctx->ABS() != nullptr) {
        auto &un = spawn<ast::UnaryExpr>(ctx);
        un.op = "abs";
        into(un.value, [&] { dispatch_(ctx->primary(0)); });
        return un;
    }
    if (ctx->NOT() != nullptr) {
        auto &un = spawn<ast::UnaryExpr>(ctx);
        un.op = "not";
        into(un.value, [&] { dispatch_(ctx->primary(0)); });
        return un;
    }

    return makePrimary(ctx->primary(0));
}

auto Translator::makeAggregate(vhdlParser::AggregateContext *ctx) -> ast::Expr &
{
    auto &group = spawn<ast::GroupExpr>(ctx);
    into(group.children, [&] {
        for (auto *elem : ctx->element_association()) {
            auto &assoc = spawn<ast::BinaryExpr>(ctx);
            assoc.op = "=>";

            if (elem->choices()) {
                into(assoc.left, [&] { makeChoices(elem->choices()); });
            }

            if (elem->expression()) {
                into(assoc.right, [&] { dispatch_(elem->expression()); });
            }
        }
    });
    return group;
}

auto Translator::makeChoices(vhdlParser::ChoicesContext *ctx) -> ast::Expr &
{
    // Multiple alternatives: choice ('|' choice)*
    if (ctx->choice().size() == 1) {
        return makeChoice(ctx->choice(0));
    }

    auto &grp = spawn<ast::GroupExpr>(ctx);
    into(grp.children, [&] {
        for (auto *ch : ctx->choice()) {
            makeChoice(ch);
        }
    });
    return grp;
}

auto Translator::makeChoice(vhdlParser::ChoiceContext *ctx) -> ast::Expr &
{
    if (ctx->OTHERS() != nullptr) {
        auto &tok = spawn<ast::TokenExpr>(ctx);
        tok.text = "others";
        return tok;
    }

    if (ctx->identifier() != nullptr) {
        auto &tok = spawn<ast::TokenExpr>(ctx);
        tok.text = ctx->identifier()->getText();
        return tok;
    }

    if (ctx->simple_expression() != nullptr) {
        return makeSimpleExpr(ctx->simple_expression());
    }

    if (auto *d_range = ctx->discrete_range()) {
        if (auto *range = d_range->range_decl()) {
            if (auto *exp_range = range->explicit_range()) {
                return makeRange(exp_range);
            }
        }
    }

    // Fallback
    auto &tok = spawn<ast::TokenExpr>(ctx);
    tok.text = ctx->getText();
    return tok;
}

auto Translator::makePrimary(vhdlParser::PrimaryContext *ctx) -> ast::Expr &
{
    if (ctx->expression() != nullptr) {
        auto &paren = spawn<ast::ParenExpr>(ctx);
        into(paren.inner, [&] { dispatch_(ctx->expression()); });
        return paren;
    }

    if (ctx->aggregate() != nullptr) {
        return makeAggregate(ctx->aggregate());
    }

    auto &tok = spawn<ast::TokenExpr>(ctx);
    tok.text = ctx->getText();
    return tok;
}

auto Translator::makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::BinaryExpr &
{
    auto &range = spawn<ast::BinaryExpr>(ctx);

    range.op = ctx->direction()->getText();

    if (auto *left = ctx->simple_expression(0)) {
        into(range.left, [&] { dispatch_(left); });
    }

    if (auto *right = ctx->simple_expression(1)) {
        into(range.right, [&] { dispatch_(right); });
    }

    return range;
}

} // namespace builder