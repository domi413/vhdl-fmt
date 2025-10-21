#include "builder/visitor.hpp"

#include "vhdlParser.h"

namespace builder {

// -----------------------------------------------------------------------------
// Entity / Architecture
// -----------------------------------------------------------------------------
void Visitor::visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx)
{
    translator_.makeEntity(ctx);
}

void Visitor::visitArchitectureBody(vhdlParser::Architecture_bodyContext *ctx)
{
    translator_.makeArchitecture(ctx);
}

// -----------------------------------------------------------------------------
// Clauses
// -----------------------------------------------------------------------------
void Visitor::visitGenericClause(vhdlParser::Generic_clauseContext *ctx)
{
    translator_.makeGenericClause(ctx);
}

void Visitor::visitPortClause(vhdlParser::Port_clauseContext *ctx)
{
    translator_.makePortClause(ctx);
}

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------
void Visitor::visitConstantDeclaration(vhdlParser::Constant_declarationContext *ctx)
{
    translator_.makeConstantDecl(ctx);
}

void Visitor::visitSignalDeclaration(vhdlParser::Signal_declarationContext *ctx)
{
    translator_.makeSignalDecl(ctx);
}

// -----------------------------------------------------------------------------
// Constraints
// -----------------------------------------------------------------------------
void Visitor::visitExplicitRange(vhdlParser::Explicit_rangeContext *ctx)
{
    translator_.makeRange(ctx);
}

// -----------------------------------------------------------------------------
// Expressions
// -----------------------------------------------------------------------------
void Visitor::visitExpression(vhdlParser::ExpressionContext *ctx)
{
    translator_.makeExpr(ctx);
}

void Visitor::visitSimpleExpression(vhdlParser::Simple_expressionContext *ctx)
{
    translator_.makeSimpleExpr(ctx);
}

void Visitor::visitAggregate(vhdlParser::AggregateContext *ctx)
{
    translator_.makeAggregate(ctx);
}

void Visitor::visitRelation(vhdlParser::RelationContext *ctx)
{
    translator_.makeRelation(ctx);
}

void Visitor::visitTerm(vhdlParser::TermContext *ctx)
{
    translator_.makeTerm(ctx);
}

void Visitor::visitFactor(vhdlParser::FactorContext *ctx)
{
    translator_.makeFactor(ctx);
}

void Visitor::visitPrimary(vhdlParser::PrimaryContext *ctx)
{
    translator_.makePrimary(ctx);
}

void Visitor::visitShiftExpression(vhdlParser::Shift_expressionContext *ctx)
{
    translator_.makeShiftExpr(ctx);
}

void Visitor::visitChoices(vhdlParser::ChoicesContext *ctx)
{
    translator_.makeChoices(ctx);
}

void Visitor::visitChoice(vhdlParser::ChoiceContext *ctx)
{
    translator_.makeChoice(ctx);
}

} // namespace builder
