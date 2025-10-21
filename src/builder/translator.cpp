#include "builder/translator.hpp"

#include "ast/nodes/declarations.hpp"
#include "ast/nodes/entity.hpp"
#include "vhdlParser.h"

namespace builder {

// ---------------------- Design units ----------------------

void Translator::makeEntity(vhdlParser::Entity_declarationContext *ctx)
{
    spawn<ast::Entity>(ctx, true, [&](auto &entity) {
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
    });
}

void Translator::makeArchitecture(vhdlParser::Architecture_bodyContext *ctx)
{
    spawn<ast::Architecture>(ctx, true, [&](auto &arch) {
        arch.name = ctx->identifier(0)->getText();
        arch.entity_name = ctx->identifier(1)->getText();

        if (auto *decl_part = ctx->architecture_declarative_part()) {
            into(arch.decls, [&] { dispatch_(decl_part); });
        }

        if (auto *stmt_part = ctx->architecture_statement_part()) {
            into(arch.stmts, [&] { dispatch_(stmt_part); });
        }
    });
}

// ------------------------ Clauses -------------------------

void Translator::makeGenericClause(vhdlParser::Generic_clauseContext *ctx)
{
    spawn<ast::GenericClause>(ctx, true, [&](auto &clause) {
        auto *list = ctx->generic_list();

        if (list == nullptr) {
            return;
        }

        into(clause.generics, [&] {
            for (auto *decl : list->interface_constant_declaration()) {
                makeGenericParam(decl);
            }
        });
    });
}

void Translator::makePortClause(vhdlParser::Port_clauseContext *ctx)
{
    spawn<ast::PortClause>(ctx, true, [&](auto &clause) {
        auto *list = ctx->port_list();
        if (list == nullptr) {
            return;
        }
        auto *iface = list->interface_port_list();
        if (iface == nullptr) {
            return;
        }
        into(clause.ports, [&] {
            for (auto *decl : iface->interface_port_declaration()) {
                makeSignalPort(decl);
            }
        });
    });
}

// ---------------------- Declarations ----------------------

void Translator::makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
{
    spawn<ast::GenericParam>(ctx, true, [&](auto &param) {
        for (auto *id_ctx : ctx->identifier_list()->identifier()) {
            param.names.emplace_back(id_ctx->getText());
        }

        if (auto *stype = ctx->subtype_indication()) {
            param.type_name = stype->getText();
        }
    });
}

void Translator::makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx)
{
    spawn<ast::Port>(ctx, true, [&](auto &port) {
        const auto &ids = ctx->identifier_list()->identifier();
        port.names.reserve(ids.size());

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
    });
}

void Translator::makeConstantDecl(vhdlParser::Constant_declarationContext *ctx)
{
    spawn<ast::ConstantDecl>(ctx, true, [&](auto &decl) {
        for (auto *id_ctx : ctx->identifier_list()->identifier()) {
            decl.names.emplace_back(id_ctx->getText());
        }

        if (auto *stype = ctx->subtype_indication()) {
            decl.type_name = stype->selected_name(0)->getText();
        }

        if (auto *expr = ctx->expression()) {
            into(decl.init_expr, [&] { dispatch_(expr); });
        }
    });
}

void Translator::makeSignalDecl(vhdlParser::Signal_declarationContext *ctx)
{
    spawn<ast::SignalDecl>(ctx, true, [&](auto &decl) {
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
                into(decl.constraints, [&] { dispatch_(constraint); });
            }
        }

        if (auto *expr = ctx->expression()) {
            into(decl.init_expr, [&] { dispatch_(expr); });
        }
    });
}

} // namespace builder
