#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_units.hpp"
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
        clause.generics.push_back(makeInterfaceConstantDecl(decl));
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
        clause.ports.push_back(makeInterfacePortDecl(decl));
    }

    return clause;
}

// ---------------------- Interface declarations ----------------------

auto Translator::makeInterfaceConstantDecl(vhdlParser::Interface_constant_declarationContext *ctx)
  -> ast::InterfaceConstantDecl
{
    auto param = make<ast::InterfaceConstantDecl>(ctx);

    param.names = ctx->identifier_list()->identifier()
                | std::views::transform([](auto *id) { return id->getText(); })
                | std::ranges::to<std::vector>();

    if (auto *stype = ctx->subtype_indication()) {
        param.type_name = stype->getText();
    }

    return param;
}

// ---------------------- Object declarations ----------------------

auto Translator::makeInterfacePortDecl(vhdlParser::Interface_port_declarationContext *ctx)
  -> ast::InterfacePortDecl
{
    auto port = make<ast::InterfacePortDecl>(ctx);

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
        port.default_expr = makeExpression(expr);
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
        decl.init_expr = makeExpression(expr);
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
        decl.init_expr = makeExpression(expr);
    }

    return decl;
}

auto Translator::makeAliasDecl(vhdlParser::Alias_declarationContext *ctx) -> ast::AliasDecl
{
    auto decl = make<ast::AliasDecl>(ctx);

    // Get the alias designator (identifier or character literal)
    if (auto *designator = ctx->alias_designator()) {
        if (auto *id = designator->identifier()) {
            decl.name = id->getText();
        } else if (auto *char_lit = designator->CHARACTER_LITERAL()) {
            decl.name = char_lit->getText();
        }
    }

    // Get optional type indication (subtype_indication or subnature_indication)
    if (auto *indication = ctx->alias_indication()) {
        decl.type_indication = (indication->subtype_indication() != nullptr)
                               ? indication->subtype_indication()->getText()
                               : indication->subnature_indication()->getText();
    }

    // Get the target name being aliased
    if (auto *name = ctx->name()) {
        decl.target = makeName(name);
    }

    // Get optional signature (for subprogram/enum disambiguation)
    if (auto *sig = ctx->signature()) {
        decl.signature = sig->getText();
    }

    return decl;
}

} // namespace builder
