#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

namespace builder {

// ---------------------- Clauses ----------------------

auto Translator::makeGenericClause(vhdlParser::Generic_clauseContext *ctx) -> ast::GenericClause
{
    ast::GenericClause clause;
    trivia_.bind(clause, ctx);

    auto *list = ctx->generic_list();
    if (list == nullptr) {
        return clause;
    }

    for (auto *decl : list->interface_constant_declaration()) {
        clause.generics.push_back(makeGenericParam(decl));
    }

    return clause;
}

auto Translator::makePortClause(vhdlParser::Port_clauseContext *ctx) -> ast::PortClause
{
    ast::PortClause clause;
    trivia_.bind(clause, ctx);

    auto *list = ctx->port_list();
    if (list == nullptr) {
        return clause;
    }

    auto *iface = list->interface_port_list();
    if (iface == nullptr) {
        return clause;
    }

    for (auto *decl : iface->interface_port_declaration()) {
        clause.ports.push_back(makeSignalPort(decl));
    }

    return clause;
}

// ---------------------- Interface declarations ----------------------

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

// ---------------------- Object declarations ----------------------

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
            port.constraints = makeConstraint(constraint);
        }
    }

    if (auto *expr = ctx->expression()) {
        port.default_expr = makeExpr(expr);
    }

    return port;
}

auto Translator::makeConstantDecl(vhdlParser::Constant_declarationContext *ctx) -> ast::ConstantDecl
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

        if (auto *constraint = stype->constraint()) {
            decl.constraints = makeConstraint(constraint);
        }
    }

    decl.has_bus_kw = false;
    if (auto *kind = ctx->signal_kind()) {
        if (kind->BUS() != nullptr) {
            decl.has_bus_kw = true;
        }
    }

    if (auto *expr = ctx->expression()) {
        decl.init_expr = makeExpr(expr);
    }

    return decl;
}

} // namespace builder