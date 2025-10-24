#include "ast/nodes/declarations.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

namespace builder {

auto Translator::makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
  -> ast::GenericParam
{
    ast::GenericParam param;
    trivia_.bind(param, ctx);
    
    for (auto *id_ctx : ctx->identifier_list()->identifier()) {
        param.names.emplace_back(id_ctx->getText());
    }

    if (auto *stype = ctx->subtype_indication()) {
        param.type_name = stype->getText();
    }
    
    return param;
}

auto Translator::makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx) -> ast::Port
{
    ast::Port port;
    trivia_.bind(port, ctx);
    
    for (auto *id_ctx : ctx->identifier_list()->identifier()) {
        port.names.emplace_back(id_ctx->getText());
    }

    if (auto *mode = ctx->signal_mode()) {
        port.mode = mode->getText();
    }

    if (auto *stype = ctx->subtype_indication()) {
        port.type_name = stype->selected_name(0)->getText();

        if (auto *constraint = stype->constraint()) {
            // TODO: Process constraints properly
            // For now, constraints are left empty - full constraint support needs implementation
            (void)constraint;
        }
    }

    if (auto *expr = ctx->expression()) {
        port.default_expr = makeExpr(expr);
    }
    
    return port;
}

auto Translator::makeConstantDecl(vhdlParser::Constant_declarationContext *ctx)
  -> ast::ConstantDecl
{
    ast::ConstantDecl decl;
    trivia_.bind(decl, ctx);
    
    for (auto *id_ctx : ctx->identifier_list()->identifier()) {
        decl.names.emplace_back(id_ctx->getText());
    }

    if (auto *stype = ctx->subtype_indication()) {
        decl.type_name = stype->selected_name(0)->getText();
    }

    if (auto *expr = ctx->expression()) {
        decl.init_expr = makeExpr(expr);
    }
    
    return decl;
}

auto Translator::makeSignalDecl(vhdlParser::Signal_declarationContext *ctx) -> ast::SignalDecl
{
    ast::SignalDecl decl;
    trivia_.bind(decl, ctx);
    
    for (auto *id_ctx : ctx->identifier_list()->identifier()) {
        decl.names.emplace_back(id_ctx->getText());
    }

    if (auto *stype = ctx->subtype_indication()) {
        decl.type_name = stype->selected_name(0)->getText();
    }

    decl.has_bus_kw = false;
    if (auto *kind = ctx->signal_kind()) {
        if (kind->BUS() != nullptr) {
            decl.has_bus_kw = true;
        }
    }

    if (auto *stype = ctx->subtype_indication()) {
        if (auto *constraint = stype->constraint()) {
            // TODO: Process constraints properly
            // For now, constraints are left empty - full constraint support needs implementation
            (void)constraint;
        }
    }

    if (auto *expr = ctx->expression()) {
        decl.init_expr = makeExpr(expr);
    }
    
    return decl;
}

} // namespace builder