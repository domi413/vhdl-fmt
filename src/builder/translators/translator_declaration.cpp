#include "ast/nodes/declarations.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <ranges>

namespace builder {

// ---------------------- Clauses ----------------------

auto Translator::makeGenericClause(vhdlParser::Generic_clauseContext *ctx) -> ast::GenericClause
{
    auto clause = make<ast::GenericClause>(ctx);

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
    auto clause = make<ast::PortClause>(ctx);

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
    auto param = make<ast::GenericParam>(ctx);

    param.names = ctx->identifier_list()->identifier()
                | std::views::transform([](auto *id) { return id->getText(); })
                | std::ranges::to<std::vector>();

    if (auto *stype = ctx->subtype_indication()) {
        param.type_name = stype->getText();
    }

    return param;
}

// ---------------------- Object declarations ----------------------

auto Translator::makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx) -> ast::Port
{
    auto port = make<ast::Port>(ctx);

    port.names = ctx->identifier_list()->identifier()
               | std::views::transform([](auto *id) { return id->getText(); })
               | std::ranges::to<std::vector>();

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
    auto decl = make<ast::ConstantDecl>(ctx);

    decl.names = ctx->identifier_list()->identifier()
               | std::views::transform([](auto *id) { return id->getText(); })
               | std::ranges::to<std::vector>();

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
    auto decl = make<ast::SignalDecl>(ctx);

    decl.names = ctx->identifier_list()->identifier()
               | std::views::transform([](auto *id) { return id->getText(); })
               | std::ranges::to<std::vector>();

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