#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <cstddef>
#include <string>
#include <utility>

// Recursion is used for traversing expression trees, which are typically shallow in VHDL parsing.
// NOLINTBEGIN(misc-no-recursion)

namespace builder {

auto Translator::makeName(vhdlParser::NameContext *ctx) -> ast::Expr
{
    // For formatting: check if we have any structural parts (calls, slices, attributes)
    // If not, just keep the whole name as a single token
    bool has_structure = false;
    for (auto *part : ctx->name_part()) {
        if (part->function_call_or_indexed_name_part()
            != nullptr
            || part->slice_name_part()
            != nullptr
            || part->attribute_name_part()
            != nullptr) {
            has_structure = true;
            break;
        }
    }

    if (!has_structure) {
        // Simple name (possibly with dots like "rec.field") - keep as one token
        return makeToken(ctx, ctx->getText());
    }

    // Has structural parts - build up the base, then apply operations
    // Start with the identifier/literal and consume any leading dot selections
    std::string base_text;
    if (ctx->identifier() != nullptr) {
        base_text = ctx->identifier()->getText();
    } else if (ctx->STRING_LITERAL() != nullptr) {
        base_text = ctx->STRING_LITERAL()->getText();
    } else {
        // Shouldn't happen, but fallback
        return makeToken(ctx, ctx->getText());
    }

    // Consume consecutive selected_name_parts into base
    size_t i = 0;
    while (i < ctx->name_part().size() && ctx->name_part()[i]->selected_name_part() != nullptr) {
        base_text += ctx->name_part()[i]->getText();
        i++;
    }

    ast::Expr base = makeToken(ctx, base_text);

    // Process remaining structural parts
    for (; i < ctx->name_part().size(); ++i) {
        auto *part = ctx->name_part()[i];

        if (auto *slice = part->slice_name_part()) {
            base = makeSliceExpr(std::move(base), slice);
        } else if (auto *call = part->function_call_or_indexed_name_part()) {
            base = makeCallExpr(std::move(base), call);
        } else if (auto *attr = part->attribute_name_part()) {
            base = makeAttributeExpr(std::move(base), attr);
        }
        // Ignore any remaining selected_name_parts (shouldn't happen after structure)
    }

    return base;
}

auto Translator::makeSliceExpr(ast::Expr base, vhdlParser::Slice_name_partContext *ctx) -> ast::Expr
{
    auto slice_expr = make<ast::CallExpr>(ctx);
    slice_expr.callee = box(std::move(base));

    if (auto *dr = ctx->discrete_range()) {
        if (auto *rd = dr->range_decl()) {
            if (auto *er = rd->explicit_range()) {
                slice_expr.args = box(makeRange(er));
            } else {
                slice_expr.args = box(makeToken(rd, rd->getText()));
            }
        } else if (auto *subtype = dr->subtype_indication()) {
            slice_expr.args = box(makeToken(subtype, subtype->getText()));
        }
    }

    return slice_expr;
}

auto Translator::makeSelectExpr(ast::Expr base, vhdlParser::Selected_name_partContext *ctx)
  -> ast::Expr
{
    return makeBinary(ctx, ".", std::move(base), makeToken(ctx, ctx->getText().substr(1)));
}

auto Translator::makeCallExpr(ast::Expr base,
                              vhdlParser::Function_call_or_indexed_name_partContext *ctx)
  -> ast::Expr
{
    auto call_expr = make<ast::CallExpr>(ctx);
    call_expr.callee = box(std::move(base));

    if (auto *assoc_list = ctx->actual_parameter_part()) {
        if (auto *list_ctx = assoc_list->association_list()) {
            auto associations = list_ctx->association_element();

            if (associations.size() == 1) {
                call_expr.args = box(makeCallArgument(associations[0]));
            } else {
                auto group = make<ast::GroupExpr>(list_ctx);
                for (auto *elem : associations) {
                    group.children.push_back(makeCallArgument(elem));
                }
                call_expr.args = box(ast::Expr{ std::move(group) });
            }
        } else {
            call_expr.args = box(makeToken(ctx, ctx->getText()));
        }
    }

    return call_expr;
}

auto Translator::makeAttributeExpr(ast::Expr base, vhdlParser::Attribute_name_partContext *ctx)
  -> ast::Expr
{
    return makeBinary(ctx, "'", std::move(base), makeToken(ctx, ctx->getText().substr(1)));
}

auto Translator::makeCallArgument(vhdlParser::Association_elementContext *ctx) -> ast::Expr
{
    if (auto *actual = ctx->actual_part()) {
        if (auto *designator = actual->actual_designator()) {
            if (auto *expr = designator->expression()) {
                return makeExpr(expr);
            }
            return makeToken(designator, designator->getText());
        }
        return makeToken(actual, actual->getText());
    }
    return makeToken(ctx, ctx->getText());
}

} // namespace builder

// NOLINTEND(misc-no-recursion)
