#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <utility>
#include <variant>
#include <vector>

namespace builder {

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
                auto range_expr = makeRange(explicit_r);
                if (auto *bin = std::get_if<ast::BinaryExpr>(&range_expr)) {
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
            auto range_expr = makeRange(explicit_r);
            if (auto *bin = std::get_if<ast::BinaryExpr>(&range_expr)) {
                constraints.push_back(std::move(*bin));
            }
        }
    }

    return constraints;
}

auto Translator::makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Expr
{
    // If there's no direction, it's just a simple expression (single value, not a range)
    if (ctx->direction() == nullptr || ctx->simple_expression().size() < 2) {
        return makeSimpleExpr(ctx->simple_expression(0));
    }

    return makeBinary(ctx,
                      ctx->direction()->getText(),
                      makeSimpleExpr(ctx->simple_expression(0)),
                      makeSimpleExpr(ctx->simple_expression(1)));
}

} // namespace builder
