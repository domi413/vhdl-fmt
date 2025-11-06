#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <ranges>
#include <utility>
#include <variant>
#include <vector>

namespace builder {

// ---------------------- Aggregates ----------------------

auto Translator::makeAggregate(vhdlParser::AggregateContext *ctx) -> ast::Expr
{
    auto group = make<ast::GroupExpr>(ctx);

    const auto &elems = ctx->element_association();
    group.children.reserve(elems.size());

    for (auto *elem : elems) {
        auto assoc = makeLight<ast::BinaryExpr>();
        // Skip trivia for internal aggregate elements
        assoc.op = "=>";

        if (auto *choices = elem->choices()) {
            assoc.left = box(makeChoices(choices));
        }
        if (auto *expr = elem->expression()) {
            assoc.right = box(makeExpr(expr));
        }

        group.children.emplace_back(std::move(assoc));
    }

    return group;
}

auto Translator::makeChoices(vhdlParser::ChoicesContext *ctx) -> ast::Expr
{
    const auto &choices = ctx->choice();
    if (choices.size() == 1) {
        return makeChoice(choices[0]);
    }

    auto grp = makeLight<ast::GroupExpr>();
    trivia_.bind(grp, ctx);
    grp.children.reserve(choices.size());
    for (auto *ch : choices) {
        grp.children.push_back(makeChoice(ch));
    }
    return grp;
}

auto Translator::makeChoice(vhdlParser::ChoiceContext *ctx) -> ast::Expr
{
    if (ctx->OTHERS() != nullptr) {
        return makeToken(ctx, "others");
    }
    if (ctx->identifier() != nullptr) {
        return makeToken(ctx, ctx->identifier()->getText());
    }
    if (ctx->simple_expression() != nullptr) {
        return makeSimpleExpr(ctx->simple_expression());
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
    const auto &discrete_ranges = ctx->discrete_range();
    std::vector<ast::BinaryExpr> constraints;
    constraints.reserve(discrete_ranges.size());

    for (auto *discrete_r : discrete_ranges) {
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

auto Translator::makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Expr
{
    const auto &simple_exprs = ctx->simple_expression();
    auto *dir = ctx->direction();

    // If there's no direction, it's just a simple expression (single value, not a range)
    if (dir == nullptr || simple_exprs.size() < 2) {
        return makeSimpleExpr(simple_exprs[0]);
    }

    return makeBinary(
      ctx, dir->getText(), makeSimpleExpr(simple_exprs[0]), makeSimpleExpr(simple_exprs[1]));
}

} // namespace builder
