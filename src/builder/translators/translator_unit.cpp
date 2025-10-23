#include "ast/nodes/design_units.hpp"
#include "builder/translator.hpp"
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

} // namespace builder
