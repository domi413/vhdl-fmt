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
    // Base name (identifier or string literal)
    ast::Expr base;
    if (ctx->identifier() != nullptr) {
        ast::TokenExpr tok;
        trivia_.bind(tok, ctx);
        tok.text = ctx->identifier()->getText();
        base = tok;
    } else if (ctx->STRING_LITERAL() != nullptr) {
        ast::TokenExpr tok;
        trivia_.bind(tok, ctx);
        tok.text = ctx->STRING_LITERAL()->getText();
        base = tok;
    } else {
        ast::TokenExpr tok;
        trivia_.bind(tok, ctx);
        tok.text = ctx->getText();
        return tok;
    }

    // Process name_parts (selections, slices, function calls, attributes)
    for (auto *part : ctx->name_part()) {
        if (auto *slice = part->slice_name_part()) {
            // This is a slice: name(range)
            ast::BinaryExpr slice_expr;
            trivia_.bind(slice_expr, part);
            slice_expr.op = "()"; // Use () as operator for slice/index
            slice_expr.left = box(std::move(base));
            if (auto *dr = slice->discrete_range()) {
                if (auto *rd = dr->range_decl()) {
                    if (auto *er = rd->explicit_range()) {
                        slice_expr.right = box(makeRange(er));
                    } else {
                        // It's a name-based range
                        ast::TokenExpr tok;
                        trivia_.bind(tok, rd);
                        tok.text = rd->getText();
                        slice_expr.right = box(ast::Expr{ std::move(tok) });
                    }
                } else if (auto *subtype = dr->subtype_indication()) {
                    ast::TokenExpr tok;
                    trivia_.bind(tok, subtype);
                    tok.text = subtype->getText();
                    slice_expr.right = box(ast::Expr{ std::move(tok) });
                }
            }
            base = ast::Expr{ std::move(slice_expr) };
        } else if (auto *selected = part->selected_name_part()) {
            // This is a selection: name.suffix
            ast::BinaryExpr sel_expr;
            trivia_.bind(sel_expr, part);
            sel_expr.op = ".";
            sel_expr.left = box(std::move(base));
            ast::TokenExpr tok;
            trivia_.bind(tok, part);
            tok.text = part->getText().substr(1); // Remove leading dot
            sel_expr.right = box(ast::Expr{ std::move(tok) });
            base = ast::Expr{ std::move(sel_expr) };
        } else {
            // For function calls, indexed names, and attributes, just append as text for now
            ast::BinaryExpr call_expr;
            trivia_.bind(call_expr, part);
            call_expr.op = "()";
            call_expr.left = box(std::move(base));
            ast::TokenExpr tok;
            trivia_.bind(tok, part);
            tok.text = part->getText();
            call_expr.right = box(ast::Expr{ std::move(tok) });
            base = ast::Expr{ std::move(call_expr) };
        }
    }

    return base;
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