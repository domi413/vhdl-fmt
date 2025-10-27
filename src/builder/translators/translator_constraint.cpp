#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <utility>
#include <variant>
#include <vector>

namespace builder {

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

} // namespace builder
