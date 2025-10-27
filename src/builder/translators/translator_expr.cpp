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

    ast::BinaryExpr bin;
    trivia_.bind(bin, ctx);
    bin.op = ctx->logical_operator(0)->getText();
    bin.left = box(makeRelation(ctx->relation(0)));
    bin.right = box(makeRelation(ctx->relation(1)));
    return bin;
}

auto Translator::makeRelation(vhdlParser::RelationContext *ctx) -> ast::Expr
{
    if (ctx->relational_operator() == nullptr) {
        return makeShiftExpr(ctx->shift_expression(0));
    }

    ast::BinaryExpr bin;
    trivia_.bind(bin, ctx);
    bin.op = ctx->relational_operator()->getText();
    bin.left = box(makeShiftExpr(ctx->shift_expression(0)));
    bin.right = box(makeShiftExpr(ctx->shift_expression(1)));
    return bin;
}

auto Translator::makeShiftExpr(vhdlParser::Shift_expressionContext *ctx) -> ast::Expr
{
    if (ctx->shift_operator() == nullptr) {
        return makeSimpleExpr(ctx->simple_expression(0));
    }

    ast::BinaryExpr bin;
    trivia_.bind(bin, ctx);
    bin.op = ctx->shift_operator()->getText();
    bin.left = box(makeSimpleExpr(ctx->simple_expression(0)));
    bin.right = box(makeSimpleExpr(ctx->simple_expression(1)));
    return bin;
}

auto Translator::makeSimpleExpr(vhdlParser::Simple_expressionContext *ctx) -> ast::Expr
{
    if ((ctx->PLUS() != nullptr) || (ctx->MINUS() != nullptr)) {
        ast::UnaryExpr un;
        trivia_.bind(un, ctx);
        un.op = (ctx->PLUS() != nullptr) ? "+" : "-";
        un.value = box(makeTerm(ctx->term(0)));
        return un;
    }

    if (ctx->adding_operator().empty()) {
        return makeTerm(ctx->term(0));
    }

    ast::BinaryExpr bin;
    trivia_.bind(bin, ctx);
    bin.op = ctx->adding_operator(0)->getText();
    bin.left = box(makeTerm(ctx->term(0)));
    bin.right = box(makeTerm(ctx->term(1)));
    return bin;
}

auto Translator::makeTerm(vhdlParser::TermContext *ctx) -> ast::Expr
{
    if (ctx->multiplying_operator().empty()) {
        return makeFactor(ctx->factor(0));
    }

    ast::BinaryExpr bin;
    trivia_.bind(bin, ctx);
    bin.op = ctx->multiplying_operator(0)->getText();
    bin.left = box(makeFactor(ctx->factor(0)));
    bin.right = box(makeFactor(ctx->factor(1)));
    return bin;
}

auto Translator::makeFactor(vhdlParser::FactorContext *ctx) -> ast::Expr
{
    if (ctx->DOUBLESTAR() != nullptr) {
        ast::BinaryExpr bin;
        trivia_.bind(bin, ctx);
        bin.op = "**";
        bin.left = box(makePrimary(ctx->primary(0)));
        bin.right = box(makePrimary(ctx->primary(1)));
        return bin;
    }

    if (ctx->ABS() != nullptr) {
        ast::UnaryExpr un;
        trivia_.bind(un, ctx);
        un.op = "abs";
        un.value = box(makePrimary(ctx->primary(0)));
        return un;
    }

    if (ctx->NOT() != nullptr) {
        ast::UnaryExpr un;
        trivia_.bind(un, ctx);
        un.op = "not";
        un.value = box(makePrimary(ctx->primary(0)));
        return un;
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

    ast::TokenExpr tok;
    trivia_.bind(tok, ctx);
    tok.text = ctx->getText();
    return tok;
}

auto Translator::makeName(vhdlParser::NameContext *ctx) -> ast::Expr
{
    // For formatting: check if we have any structural parts (calls, slices, attributes)
    // If not, just keep the whole name as a single token
    bool has_structure = false;
    for (auto *part : ctx->name_part()) {
        if (part->function_call_or_indexed_name_part() != nullptr ||
            part->slice_name_part() != nullptr || part->attribute_name_part() != nullptr) {
            has_structure = true;
            break;
        }
    }

    if (!has_structure) {
        // Simple name (possibly with dots like "rec.field") - keep as one token
        ast::TokenExpr tok;
        trivia_.bind(tok, ctx);
        tok.text = ctx->getText();
        return tok;
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
        ast::TokenExpr tok;
        trivia_.bind(tok, ctx);
        tok.text = ctx->getText();
        return tok;
    }

    // Consume consecutive selected_name_parts into base
    size_t i = 0;
    while (i < ctx->name_part().size() &&
           ctx->name_part()[i]->selected_name_part() != nullptr) {
        base_text += ctx->name_part()[i]->getText();
        i++;
    }

    ast::TokenExpr base_tok;
    trivia_.bind(base_tok, ctx);
    base_tok.text = base_text;
    ast::Expr base = base_tok;

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

auto Translator::makeSliceExpr(ast::Expr base, vhdlParser::Slice_name_partContext *ctx)
  -> ast::Expr
{
    ast::CallExpr slice_expr;
    trivia_.bind(slice_expr, ctx);
    slice_expr.callee = box(std::move(base));

    if (auto *dr = ctx->discrete_range()) {
        if (auto *rd = dr->range_decl()) {
            if (auto *er = rd->explicit_range()) {
                slice_expr.args = box(makeRange(er));
            } else {
                // It's a name-based range
                ast::TokenExpr tok;
                trivia_.bind(tok, rd);
                tok.text = rd->getText();
                slice_expr.args = box(ast::Expr{ std::move(tok) });
            }
        } else if (auto *subtype = dr->subtype_indication()) {
            ast::TokenExpr tok;
            trivia_.bind(tok, subtype);
            tok.text = subtype->getText();
            slice_expr.args = box(ast::Expr{ std::move(tok) });
        }
    }

    return slice_expr;
}

auto Translator::makeSelectExpr(ast::Expr base, vhdlParser::Selected_name_partContext *ctx)
  -> ast::Expr
{
    ast::BinaryExpr sel_expr;
    trivia_.bind(sel_expr, ctx);
    sel_expr.op = ".";
    sel_expr.left = box(std::move(base));

    ast::TokenExpr tok;
    trivia_.bind(tok, ctx);
    tok.text = ctx->getText().substr(1); // Remove leading dot
    sel_expr.right = box(ast::Expr{ std::move(tok) });

    return sel_expr;
}

auto Translator::makeCallExpr(ast::Expr base,
                               vhdlParser::Function_call_or_indexed_name_partContext *ctx)
  -> ast::Expr
{
    ast::CallExpr call_expr;
    trivia_.bind(call_expr, ctx);
    call_expr.callee = box(std::move(base));

    // Parse the actual parameter part (association list)
    if (auto *assoc_list = ctx->actual_parameter_part()) {
        if (auto *list_ctx = assoc_list->association_list()) {
            auto associations = list_ctx->association_element();

            if (associations.size() == 1) {
                // Single argument - extract it directly
                call_expr.args = box(makeCallArgument(associations[0]));
            } else {
                // Multiple arguments - create a group
                ast::GroupExpr group;
                trivia_.bind(group, list_ctx);
                for (auto *elem : associations) {
                    group.children.push_back(makeCallArgument(elem));
                }
                call_expr.args = box(ast::Expr{ std::move(group) });
            }
        } else {
            // Fallback: use raw text
            ast::TokenExpr tok;
            trivia_.bind(tok, ctx);
            tok.text = ctx->getText();
            call_expr.args = box(ast::Expr{ std::move(tok) });
        }
    }

    return call_expr;
}

auto Translator::makeAttributeExpr(ast::Expr base, vhdlParser::Attribute_name_partContext *ctx)
  -> ast::Expr
{
    ast::BinaryExpr attr_expr;
    trivia_.bind(attr_expr, ctx);
    attr_expr.op = "'";
    attr_expr.left = box(std::move(base));

    ast::TokenExpr tok;
    trivia_.bind(tok, ctx);
    tok.text = ctx->getText().substr(1); // Remove leading apostrophe
    attr_expr.right = box(ast::Expr{ std::move(tok) });

    return attr_expr;
}

auto Translator::makeCallArgument(vhdlParser::Association_elementContext *ctx) -> ast::Expr
{
    if (auto *actual = ctx->actual_part()) {
        if (auto *designator = actual->actual_designator()) {
            if (auto *expr = designator->expression()) {
                return makeExpr(expr);
            }
            // OPEN keyword or other
            ast::TokenExpr tok;
            trivia_.bind(tok, designator);
            tok.text = designator->getText();
            return tok;
        }
        // Fallback
        ast::TokenExpr tok;
        trivia_.bind(tok, actual);
        tok.text = actual->getText();
        return tok;
    }
    // Empty - shouldn't happen
    ast::TokenExpr tok;
    trivia_.bind(tok, ctx);
    tok.text = ctx->getText();
    return tok;
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
        ast::TokenExpr tok;
        trivia_.bind(tok, ctx);
        tok.text = "others";
        return tok;
    }

    if (ctx->identifier() != nullptr) {
        ast::TokenExpr tok;
        trivia_.bind(tok, ctx);
        tok.text = ctx->identifier()->getText();
        return tok;
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

    ast::TokenExpr tok;
    trivia_.bind(tok, ctx);
    tok.text = ctx->getText();
    return tok;
}

auto Translator::makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Expr
{
    // If there's no direction, it's just a simple expression (single value, not a range)
    if (ctx->direction() == nullptr || ctx->simple_expression().size() < 2) {
        return makeSimpleExpr(ctx->simple_expression(0));
    }

    ast::BinaryExpr range;
    trivia_.bind(range, ctx);
    range.op = ctx->direction()->getText();
    range.left = box(makeSimpleExpr(ctx->simple_expression(0)));
    range.right = box(makeSimpleExpr(ctx->simple_expression(1)));
    return range;
}

} // namespace builder

// NOLINTEND(misc-no-recursion)