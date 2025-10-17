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
// void Visitor::visitConstraint(vhdlParser::ConstraintContext *ctx)
// {
//     auto *index_con = ctx->index_constraint();
//     if (index_con == nullptr) {
//         return;
//     }
//     for (auto *range_ctx : index_con->discrete_range()) {
//         auto *range_decl = range_ctx->range_decl();
//         if (range_decl == nullptr) {
//             continue;
//         }
//         if (auto *explicit_range = range_decl->explicit_range()) {
//             translator_.makeRange(explicit_range);
//         }
//     }
// }

void Visitor::visitExplicitRange(vhdlParser::Explicit_rangeContext *ctx)
{
    translator_.makeRange(ctx);
}

// -----------------------------------------------------------------------------
// Expressions
// -----------------------------------------------------------------------------
void Visitor::visitSimpleExpression(vhdlParser::Simple_expressionContext *ctx)
{
    translator_.makeExpr(ctx);
}

} // namespace builder
