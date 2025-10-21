#include "ast/nodes/expressions.hpp"

#include "builder/translator.hpp"
#include "vhdlParser.h"

// NOLINTBEGIN(misc-no-recursion)

namespace builder {

void Translator::makeExpr(vhdlParser::ExpressionContext *ctx)
{
    if (ctx->relation().size() == 1) {
        makeRelation(ctx->relation(0));
        return;
    }

    spawn<ast::BinaryExpr>(ctx, true, [&](auto &bin) {
        bin.op = ctx->logical_operator(0)->getText();
        into(bin.left, [&] { makeRelation(ctx->relation(0)); });
        into(bin.right, [&] { makeRelation(ctx->relation(1)); });
    });
}

void Translator::makeRelation(vhdlParser::RelationContext *ctx)
{
    if (ctx->relational_operator() == nullptr) {
        makeShiftExpr(ctx->shift_expression(0));
        return;
    }

    spawn<ast::BinaryExpr>(ctx, true, [&](auto &bin) {
        bin.op = ctx->relational_operator()->getText();
        into(bin.left, [&] { makeShiftExpr(ctx->shift_expression(0)); });
        into(bin.right, [&] { makeShiftExpr(ctx->shift_expression(1)); });
    });
}

void Translator::makeShiftExpr(vhdlParser::Shift_expressionContext *ctx)
{
    if (ctx->shift_operator() == nullptr) {
        makeSimpleExpr(ctx->simple_expression(0));
        return;
    }

    spawn<ast::BinaryExpr>(ctx, true, [&](auto &bin) {
        bin.op = ctx->shift_operator()->getText();
        into(bin.left, [&] { makeSimpleExpr(ctx->simple_expression(0)); });
        into(bin.right, [&] { makeSimpleExpr(ctx->simple_expression(1)); });
    });
}

void Translator::makeSimpleExpr(vhdlParser::Simple_expressionContext *ctx)
{
    if ((ctx->PLUS() != nullptr) || (ctx->MINUS() != nullptr)) {
        spawn<ast::UnaryExpr>(ctx, true, [&](auto &un) {
            un.op = ctx->PLUS() ? "+" : "-";
            into(un.value, [&] { makeTerm(ctx->term(0)); });
        });
        return;
    }

    if (ctx->adding_operator().empty()) {
        makeTerm(ctx->term(0));
        return;
    }

    spawn<ast::BinaryExpr>(ctx, true, [&](auto &bin) {
        bin.op = ctx->adding_operator(0)->getText();
        into(bin.left, [&] { makeTerm(ctx->term(0)); });
        into(bin.right, [&] { makeTerm(ctx->term(1)); });
    });
}

void Translator::makeTerm(vhdlParser::TermContext *ctx)
{
    if (ctx->multiplying_operator().empty()) {
        makeFactor(ctx->factor(0));
        return;
    }

    spawn<ast::BinaryExpr>(ctx, true, [&](auto &bin) {
        bin.op = ctx->multiplying_operator(0)->getText();
        into(bin.left, [&] { makeFactor(ctx->factor(0)); });
        into(bin.right, [&] { makeFactor(ctx->factor(1)); });
    });
}

void Translator::makeFactor(vhdlParser::FactorContext *ctx)
{
    if (ctx->DOUBLESTAR() != nullptr) {
        spawn<ast::BinaryExpr>(ctx, true, [&](auto &bin) {
            bin.op = "**";
            into(bin.left, [&] { makePrimary(ctx->primary(0)); });
            into(bin.right, [&] { makePrimary(ctx->primary(1)); });
        });
        return;
    }

    if (ctx->ABS() != nullptr) {
        spawn<ast::UnaryExpr>(ctx, true, [&](auto &un) {
            un.op = "abs";
            into(un.value, [&] { makePrimary(ctx->primary(0)); });
        });
        return;
    }

    if (ctx->NOT() != nullptr) {
        spawn<ast::UnaryExpr>(ctx, true, [&](auto &un) {
            un.op = "not";
            into(un.value, [&] { makePrimary(ctx->primary(0)); });
        });
        return;
    }

    makePrimary(ctx->primary(0));
}

void Translator::makePrimary(vhdlParser::PrimaryContext *ctx)
{
    if (ctx->expression() != nullptr) {
        spawn<ast::ParenExpr>(
          ctx, true, [&](auto &p) { into(p.inner, [&] { makeExpr(ctx->expression()); }); });
        return;
    }

    if (ctx->aggregate() != nullptr) {
        makeAggregate(ctx->aggregate());
        return;
    }

    spawn<ast::TokenExpr>(ctx, true, [&](auto &tok) { tok.text = ctx->getText(); });
}

void Translator::makeAggregate(vhdlParser::AggregateContext *ctx)
{
    spawn<ast::GroupExpr>(ctx, true, [&](auto &group) {
        into(group.children, [&] {
            for (auto *elem : ctx->element_association()) {
                spawn<ast::BinaryExpr>(ctx, true, [&](auto &assoc) {
                    assoc.op = "=>";
                    if (elem->choices()) {
                        into(assoc.left, [&] { makeChoices(elem->choices()); });
                    }
                    if (elem->expression()) {
                        into(assoc.right, [&] { makeExpr(elem->expression()); });
                    }
                });
            }
        });
    });
}

void Translator::makeChoices(vhdlParser::ChoicesContext *ctx)
{
    if (ctx->choice().size() == 1) {
        makeChoice(ctx->choice(0));
        return;
    }

    spawn<ast::GroupExpr>(ctx, true, [&](auto &grp) {
        into(grp.children, [&] {
            for (auto *ch : ctx->choice()) {
                makeChoice(ch);
            }
        });
    });
}

void Translator::makeChoice(vhdlParser::ChoiceContext *ctx)
{
    if (ctx->OTHERS() != nullptr) {
        spawn<ast::TokenExpr>(ctx, true, [&](auto &t) { t.text = "others"; });
        return;
    }

    if (ctx->identifier() != nullptr) {
        spawn<ast::TokenExpr>(ctx, true, [&](auto &t) { t.text = ctx->identifier()->getText(); });
        return;
    }

    if (ctx->simple_expression() != nullptr) {
        makeSimpleExpr(ctx->simple_expression());
        return;
    }

    if (auto *dr = ctx->discrete_range()) {
        if (auto *rd = dr->range_decl()) {
            if (auto *er = rd->explicit_range()) {
                makeRange(er);
                return;
            }
        }
    }

    spawn<ast::TokenExpr>(ctx, true, [&](auto &t) { t.text = ctx->getText(); });
}

void Translator::makeRange(vhdlParser::Explicit_rangeContext *ctx)
{
    spawn<ast::BinaryExpr>(ctx, true, [&](auto &range) {
        range.op = ctx->direction()->getText();
        into(range.left, [&] { dispatch_(ctx->simple_expression(0)); });
        into(range.right, [&] { dispatch_(ctx->simple_expression(1)); });
    });
}

} // namespace builder

// NOLINTEND(misc-no-recursion)