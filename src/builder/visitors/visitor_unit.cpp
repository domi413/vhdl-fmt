#include "builder/visitor.hpp"
#include "vhdlParser.h"

namespace builder {

// ---------------------- Design units ----------------------

void Visitor::visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx)
{
    translator_.makeEntity(ctx);
}

void Visitor::visitArchitectureBody(vhdlParser::Architecture_bodyContext *ctx)
{
    translator_.makeArchitecture(ctx);
}

// ------------------------ Clauses -------------------------

void Visitor::visitGenericClause(vhdlParser::Generic_clauseContext *ctx)
{
    translator_.makeGenericClause(ctx);
}

void Visitor::visitPortClause(vhdlParser::Port_clauseContext *ctx)
{
    translator_.makePortClause(ctx);
}

} // namespace builder