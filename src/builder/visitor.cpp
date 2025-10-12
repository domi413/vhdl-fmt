#include "visitor.hpp"

#include "vhdlParser.h"

namespace builder {

void Visitor::visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx)
{
    auto &entity{ translator.makeEntity(ctx) };
    if (auto *header = ctx->entity_header()) {
        // GENERICS
        translator.into(entity.generics, [&] -> void { dispatch(header->generic_clause()); });

        // PORTS
        translator.into(entity.ports, [&] -> void { dispatch(header->port_clause()); });
    }
}

void Visitor::visitGenericClause(vhdlParser::Generic_clauseContext *ctx)
{
    auto *g_list{ ctx->generic_list() };
    if (g_list == nullptr) {
        return;
    }
    for (auto *i_c_decl : g_list->interface_constant_declaration()) {
        translator.makeGenericParam(i_c_decl);
    }
}

void Visitor::visitPortClause(vhdlParser::Port_clauseContext *ctx)
{
    auto *p_list{ ctx->port_list() };
    if (p_list == nullptr) {
        return;
    }
    if (auto *i_p_list = p_list->interface_port_list()) {
        for (auto *i_p_decl : i_p_list->interface_port_declaration()) {
            auto &port = translator.makeSignalPort(i_p_decl);
            if (auto *s_type = i_p_decl->subtype_indication()) {
                if (auto *con = s_type->constraint()) {
                    translator.into(port.constraints, [&] -> void { dispatch(con); });
                }
            }
        }
    }
}

void Visitor::visitConstraint(vhdlParser::ConstraintContext *ctx)
{
    if (auto *i_con = ctx->index_constraint()) {
        for (auto *d_range : i_con->discrete_range()) {
            if (auto *r_decl = d_range->range_decl()) {
                if (auto *erange = r_decl->explicit_range()) {
                    translator.makeRange(erange);
                }
            }
        }
    }
}

} // namespace builder
