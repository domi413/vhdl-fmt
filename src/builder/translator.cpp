#include "builder/translator.hpp"

#include "ast/nodes/declarations.hpp"
#include "ast/nodes/entity.hpp"
#include "ast/nodes/ranges.hpp"
#include "vhdlParser.h"

namespace builder {

// ---------------------- Design units ----------------------

auto Translator::makeEntity(vhdlParser::Entity_declarationContext *ctx) -> ast::Entity &
{
    auto &entity = spawn<ast::Entity>(ctx);
    entity.name = ctx->identifier(0)->getText();

    // Optional end label (ENTITY ... END ENTITY <id>)
    if (ctx->identifier().size() > 1) {
        entity.end_label = ctx->identifier(1)->getText();
    }

    if (auto *header = ctx->entity_header()) {
        if (auto *gen_clause = header->generic_clause()) {
            into(entity.generic_clause, [&] { dispatch_(gen_clause); });
        }
        if (auto *port_clause = header->port_clause()) {
            into(entity.port_clause, [&] { dispatch_(port_clause); });
        }
    }

    return entity;
}

auto Translator::makeArchitecture(vhdlParser::Architecture_bodyContext *ctx) -> ast::Architecture &
{
    auto &arch = spawn<ast::Architecture>(ctx);
    arch.name = ctx->identifier(0)->getText();
    arch.entity_name = ctx->identifier(1)->getText();

    if (auto *decl_part = ctx->architecture_declarative_part()) {
        into(arch.decls, [&] { dispatch_(decl_part); });
    }

    if (auto *stmt_part = ctx->architecture_statement_part()) {
        into(arch.stmts, [&] { dispatch_(stmt_part); });
    }

    return arch;
}

// ------------------------ Clauses -------------------------

auto Translator::makeGenericClause(vhdlParser::Generic_clauseContext *ctx) -> ast::GenericClause &
{
    auto &clause = spawn<ast::GenericClause>(ctx);

    auto *list = ctx->generic_list();

    if (list == nullptr) {
        return clause;
    }

    into(clause.generics, [&] {
        for (auto *decl : list->interface_constant_declaration()) {
            auto &param = makeGenericParam(decl);

            if (auto *expr = decl->expression()) {
                into(param.default_expr, [&] { dispatch_(expr); });
            }
        }
    });

    return clause;
}

auto Translator::makePortClause(vhdlParser::Port_clauseContext *ctx) -> ast::PortClause &
{
    auto &clause = spawn<ast::PortClause>(ctx);

    auto *list = ctx->port_list();
    if (list == nullptr) {
        return clause;
    }
    auto *iface = list->interface_port_list();
    if (iface == nullptr) {
        return clause;
    }
    into(clause.ports, [&] {
        for (auto *decl : iface->interface_port_declaration()) {
            auto &port = makeSignalPort(decl);
        }
    });

    return clause;
}

// ---------------------- Declarations ----------------------

auto Translator::makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
  -> ast::GenericParam &
{
    auto &param = spawn<ast::GenericParam>(ctx);

    for (auto *id_ctx : ctx->identifier_list()->identifier()) {
        param.names.emplace_back(id_ctx->getText());
    }

    if (auto *stype = ctx->subtype_indication()) {
        param.type_name = stype->getText();
    }

    return param;
}

auto Translator::makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx) -> ast::Port &
{
    auto &port = spawn<ast::Port>(ctx);

    for (auto *id_ctx : ctx->identifier_list()->identifier()) {
        port.names.emplace_back(id_ctx->getText());
    }

    if (auto *mode = ctx->signal_mode()) {
        port.mode = mode->getText();
    }

    if (auto *stype = ctx->subtype_indication()) {
        port.type_name = stype->selected_name(0)->getText();

        if (auto *constraint = stype->constraint()) {
            into(port.constraints, [&] { dispatch_(constraint); });
        }
    }

    if (auto *expr = ctx->expression()) {
        into(port.default_expr, [&] { dispatch_(expr); });
    }

    return port;
}

auto Translator::makeConstantDecl(vhdlParser::Constant_declarationContext *ctx)
  -> ast::ConstantDecl &
{
    auto &decl = spawn<ast::ConstantDecl>(ctx);

    for (auto *id_ctx : ctx->identifier_list()->identifier()) {
        decl.names.emplace_back(id_ctx->getText());
    }

    if (auto *stype = ctx->subtype_indication()) {
        decl.type_name = stype->selected_name(0)->getText();
    }

    if (auto *expr = ctx->expression()) {
        decl.init_expr = expr->getText();
    }

    return decl;
}

auto Translator::makeSignalDecl(vhdlParser::Signal_declarationContext *ctx) -> ast::SignalDecl &
{
    auto &decl = spawn<ast::SignalDecl>(ctx);

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

    into(decl.constraints, [&] {
        if (auto *stype = ctx->subtype_indication()) {
            if (auto *constraint = stype->constraint()) {
                dispatch_(constraint);
            }
        }
    });

    into(decl.init_expr, [&] {
        if (auto *expr = ctx->expression()) {
            dispatch_(expr);
        }
    });

    return decl;
}

auto Translator::makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Range &
{
    auto &range = spawn<ast::Range>(ctx);

    range.direction = ctx->direction()->getText();

    into(range.left, [&] {
        if (auto *left = ctx->simple_expression(0)) {
            dispatch_(left);
        }
    });

    into(range.right, [&] {
        if (auto *right = ctx->simple_expression(1)) {
            dispatch_(right);
        }
    });

    return range;
}

} // namespace builder
