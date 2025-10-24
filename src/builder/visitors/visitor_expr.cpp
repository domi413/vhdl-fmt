#include "builder/visitor.hpp"
#include "vhdlParser.h"

namespace builder {

void Visitor::visitExpression(vhdlParser::ExpressionContext *ctx)
{
    (void)translator_.makeExpr(ctx);
}

void Visitor::visitSimpleExpression(vhdlParser::Simple_expressionContext *ctx)
{
    (void)translator_.makeSimpleExpr(ctx);
}

void Visitor::visitAggregate(vhdlParser::AggregateContext *ctx)
{
    (void)translator_.makeAggregate(ctx);
}

void Visitor::visitRelation(vhdlParser::RelationContext *ctx)
{
    (void)translator_.makeRelation(ctx);
}

void Visitor::visitTerm(vhdlParser::TermContext *ctx)
{
    (void)translator_.makeTerm(ctx);
}

void Visitor::visitFactor(vhdlParser::FactorContext *ctx)
{
    (void)translator_.makeFactor(ctx);
}

void Visitor::visitPrimary(vhdlParser::PrimaryContext *ctx)
{
    (void)translator_.makePrimary(ctx);
}

void Visitor::visitShiftExpression(vhdlParser::Shift_expressionContext *ctx)
{
    (void)translator_.makeShiftExpr(ctx);
}

void Visitor::visitChoices(vhdlParser::ChoicesContext *ctx)
{
    (void)translator_.makeChoices(ctx);
}

void Visitor::visitChoice(vhdlParser::ChoiceContext *ctx)
{
    (void)translator_.makeChoice(ctx);
}

void Visitor::visitExplicitRange(vhdlParser::Explicit_rangeContext *ctx)
{
    (void)translator_.makeRange(ctx);
}

} // namespace builder
