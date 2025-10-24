#include "builder/visitor.hpp"
#include "vhdlParser.h"

namespace builder {

// ---------------------- Design units ----------------------

void Visitor::visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx)
{
    // Return value needs to be collected by the caller
    // For now, we need a way to add it to the root
    (void)translator_.makeEntity(ctx);
}

void Visitor::visitArchitectureBody(vhdlParser::Architecture_bodyContext *ctx)
{
    (void)translator_.makeArchitecture(ctx);
}

// ------------------------ Clauses -------------------------

void Visitor::visitGenericClause(vhdlParser::Generic_clauseContext *ctx)
{
    (void)translator_.makeGenericClause(ctx);
}

void Visitor::visitPortClause(vhdlParser::Port_clauseContext *ctx)
{
    (void)translator_.makePortClause(ctx);
}

} // namespace builder