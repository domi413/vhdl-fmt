#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <ranges>
#include <utility>
#include <variant>
#include <vector>

namespace builder {

// ---------------------- Aggregates ----------------------

auto Translator::makeAggregate(vhdlParser::AggregateContext *ctx) -> ast::Expression
{
    auto group = make<ast::GroupExpr>(ctx);

    for (auto *elem : ctx->element_association()) {
        auto assoc = make<ast::BinaryExpr>(elem);
        assoc.op = "=>";

        if (elem->choices() != nullptr) {
            assoc.left = box(makeChoices(elem->choices()));
        }
        if (elem->expression() != nullptr) {
            assoc.right = box(makeExpression(elem->expression()));
        }

        group.children.emplace_back(std::move(assoc));
    }

    return group;
}

auto Translator::makeChoices(vhdlParser::ChoicesContext *ctx) -> ast::Expression
{
    if (ctx->choice().size() == 1) {
        return makeChoice(ctx->choice(0));
    }

    auto grp = make<ast::GroupExpr>(ctx);
    grp.children = ctx->choice()
                 | std::views::transform([this](auto *ch) { return makeChoice(ch); })
                 | std::ranges::to<std::vector>();
    return grp;
}

auto Translator::makeChoices(vhdlParser::ChoiceContext *ctx) -> ast::Expression
{
    if (ctx->OTHERS() != nullptr) {
        return makeToken(ctx, "others");
    }
    if (ctx->identifier() != nullptr) {
        return makeToken(ctx, ctx->identifier()->getText());
    }
    if (ctx->simple_expression() != nullptr) {
        return makeSimpleExpression(ctx->simple_expression());
    }
    if (auto *dr = ctx->discrete_range()) {
        if (auto *rd = dr->range_decl()) {
            if (auto *er = rd->explicit_range()) {
                return makeRange(er);
            }
        }
    }
    return makeToken(ctx, ctx->getText());
}

// ---------------------- Constraints/Ranges ----------------------

auto Translator::makeConstraint(vhdlParser::ConstraintContext *ctx) -> std::vector<ast::BinaryExpr>
{
    // Dispatch based on concrete constraint type
    if (auto *index = ctx->index_constraint()) {
        return makeIndexConstraint(index);
    }
    if (auto *range = ctx->range_constraint()) {
        return makeRangeConstraint(range);
    }

    // Fallback: return empty vector
    return {};
}

auto Translator::makeIndexConstraint(vhdlParser::Index_constraintContext *ctx)
  -> std::vector<ast::BinaryExpr>
{
    std::vector<ast::BinaryExpr> constraints;

    for (auto *discrete_r : ctx->discrete_range()) {
        if (auto *range_decl = discrete_r->range_decl()) {
            if (auto *explicit_r = range_decl->explicit_range()) {
                if (auto range_expr = makeRange(explicit_r);
                    auto *bin = std::get_if<ast::BinaryExpr>(&range_expr)) {
                    constraints.push_back(std::move(*bin));
                }
            }
        }
    }

    return constraints;
}

auto Translator::makeRangeConstraint(vhdlParser::Range_constraintContext *ctx)
  -> std::vector<ast::BinaryExpr>
{
    std::vector<ast::BinaryExpr> constraints;

    if (auto *range_decl = ctx->range_decl()) {
        if (auto *explicit_r = range_decl->explicit_range()) {
            if (auto range_expr = makeRange(explicit_r);
                auto *bin = std::get_if<ast::BinaryExpr>(&range_expr)) {
                constraints.push_back(std::move(*bin));
            }
        }
    }

    return constraints;
}

auto Translator::makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Expression
{
    // If there's no direction, it's just a simple expression (single value, not a range)
    if (ctx->direction() == nullptr || ctx->simple_expression().size() < 2) {
        return makeSimpleExpression(ctx->simple_expression(0));
    }

    return makeBinary(ctx,
                      ctx->direction()->getText(),
                      makeSimpleExpression(ctx->simple_expression(0)),
                      makeSimpleExpression(ctx->simple_expression(1)));
}

} // namespace builder
