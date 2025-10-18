#include "builder/visitor.hpp"

#include "vhdlParser.h"

namespace builder {

// -----------------------------------------------------------------------------
// Entity / Architecture
// -----------------------------------------------------------------------------
void Visitor::visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx)
{
    auto &entity = translator_.makeEntity(ctx);
}

void Visitor::visitArchitectureBody(vhdlParser::Architecture_bodyContext *ctx)
{
    auto &arch = translator_.makeArchitecture(ctx);
}

// -----------------------------------------------------------------------------
// Clauses
// -----------------------------------------------------------------------------
void Visitor::visitGenericClause(vhdlParser::Generic_clauseContext *ctx)
{
    auto &clause = translator_.makeGenericClause(ctx);
}

void Visitor::visitPortClause(vhdlParser::Port_clauseContext *ctx)
{
    auto &clause = translator_.makePortClause(ctx);
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

} // namespace builder
