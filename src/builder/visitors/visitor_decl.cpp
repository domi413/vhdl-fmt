#include "builder/visitor.hpp"
#include "vhdlParser.h"

namespace builder {

void Visitor::visitConstantDeclaration(vhdlParser::Constant_declarationContext *ctx)
{
    auto decl = translator_.makeConstantDecl(ctx);
    translator_.pushDecl(std::move(decl));
}

void Visitor::visitSignalDeclaration(vhdlParser::Signal_declarationContext *ctx)
{
    auto decl = translator_.makeSignalDecl(ctx);
    translator_.pushDecl(std::move(decl));
}

} // namespace builder
