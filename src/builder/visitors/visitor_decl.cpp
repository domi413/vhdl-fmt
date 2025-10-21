#include "builder/visitor.hpp"
#include "vhdlParser.h"

namespace builder {

void Visitor::visitConstantDeclaration(vhdlParser::Constant_declarationContext *ctx)
{
    translator_.makeConstantDecl(ctx);
}

void Visitor::visitSignalDeclaration(vhdlParser::Signal_declarationContext *ctx)
{
    translator_.makeSignalDecl(ctx);
}

} // namespace builder
