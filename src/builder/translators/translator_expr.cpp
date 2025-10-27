#include "ast/nodes/expressions.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <utility>

// NOLINTBEGIN(misc-no-recursion)

namespace builder {

auto Translator::makeExpr(vhdlParser::ExpressionContext *ctx) -> ast::Expr
{
    if (ctx->relation().size() == 1) {
        return makeRelation(ctx->relation(0));
    }
    return makeBinary(ctx,
                      ctx->logical_operator(0)->getText(),
                      makeRelation(ctx->relation(0)),
                      makeRelation(ctx->relation(1)));
}

auto Translator::makeRelation(vhdlParser::RelationContext *ctx) -> ast::Expr
{
    if (ctx->relational_operator() == nullptr) {
        return makeShiftExpr(ctx->shift_expression(0));
    }
    return makeBinary(ctx,
                      ctx->relational_operator()->getText(),
                      makeShiftExpr(ctx->shift_expression(0)),
                      makeShiftExpr(ctx->shift_expression(1)));
}

auto Translator::makeShiftExpr(vhdlParser::Shift_expressionContext *ctx) -> ast::Expr
{
    if (ctx->shift_operator() == nullptr) {
        return makeSimpleExpr(ctx->simple_expression(0));
    }
    return makeBinary(ctx,
                      ctx->shift_operator()->getText(),
                      makeSimpleExpr(ctx->simple_expression(0)),
                      makeSimpleExpr(ctx->simple_expression(1)));
}

auto Translator::makeSimpleExpr(vhdlParser::Simple_expressionContext *ctx) -> ast::Expr
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

auto Translator::makeTerm(vhdlParser::TermContext *ctx) -> ast::Expr
{
    if (ctx->multiplying_operator().empty()) {
        return makeFactor(ctx->factor(0));
    }
    return makeBinary(ctx,
                      ctx->multiplying_operator(0)->getText(),
                      makeFactor(ctx->factor(0)),
                      makeFactor(ctx->factor(1)));
}

auto Translator::makeFactor(vhdlParser::FactorContext *ctx) -> ast::Expr
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

auto Translator::makePrimary(vhdlParser::PrimaryContext *ctx) -> ast::Expr
{
    if (ctx->expression() != nullptr) {
        ast::ParenExpr paren;
        trivia_.bind(paren, ctx);
        paren.inner = box(makeExpr(ctx->expression()));
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
    ast::CallExpr slice_expr;
    trivia_.bind(slice_expr, ctx);
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
    ast::CallExpr call_expr;
    trivia_.bind(call_expr, ctx);
    call_expr.callee = box(std::move(base));

    if (auto *assoc_list = ctx->actual_parameter_part()) {
        if (auto *list_ctx = assoc_list->association_list()) {
            auto associations = list_ctx->association_element();

            if (associations.size() == 1) {
                call_expr.args = box(makeCallArgument(associations[0]));
            } else {
                ast::GroupExpr group;
                trivia_.bind(group, list_ctx);
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

auto Translator::makeAggregate(vhdlParser::AggregateContext *ctx) -> ast::Expr
{
    ast::GroupExpr group;
    trivia_.bind(group, ctx);

    for (auto *elem : ctx->element_association()) {
        ast::BinaryExpr assoc;
        trivia_.bind(assoc, elem);
        assoc.op = "=>";

        if (elem->choices() != nullptr) {
            assoc.left = box(makeChoices(elem->choices()));
        }
        if (elem->expression() != nullptr) {
            assoc.right = box(makeExpr(elem->expression()));
        }

        group.children.emplace_back(std::move(assoc));
    }

    return group;
}

auto Translator::makeChoices(vhdlParser::ChoicesContext *ctx) -> ast::Expr
{
    if (ctx->choice().size() == 1) {
        return makeChoice(ctx->choice(0));
    }

    ast::GroupExpr grp;
    trivia_.bind(grp, ctx);
    for (auto *ch : ctx->choice()) {
        grp.children.emplace_back(makeChoice(ch));
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

} // namespace builder

// NOLINTEND(misc-no-recursion)