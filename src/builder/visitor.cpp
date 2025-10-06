#include "visitor.hpp"

namespace builder {

void Visitor::visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx)
{
    auto &entity = translator.makeEntity(ctx);
    if (auto *header = ctx->entity_header()) {
        // GENERICS
        translator.into(entity.generics, [&] { dispatch(header->generic_clause()); });

        // PORTS
        translator.into(entity.ports, [&] { dispatch(header->port_clause()); });
    }
}

void Visitor::visitGenericClause(vhdlParser::Generic_clauseContext *ctx)
{
    auto *glist = ctx->generic_list();
    if (!glist) {
        return;
    }
    for (auto *icdecl : glist->interface_constant_declaration()) {
        translator.makeGenericParam(icdecl);
    }
}

void Visitor::visitPortClause(vhdlParser::Port_clauseContext *ctx)
{
    auto *plist = ctx->port_list();
    if (!plist) {
        return;
    }
    if (auto *iplist_ctx = plist->interface_port_list()) {
        for (auto *ipdecl : iplist_ctx->interface_port_declaration()) {
            auto &port = translator.makeSignalPort(ipdecl);
            if (auto *stype = ipdecl->subtype_indication()) {
                if (auto *con = stype->constraint()) {
                    translator.into(port.constraints, [&] { dispatch(con); });
                }
            }
        }
    }
}

void Visitor::visitConstraint(vhdlParser::ConstraintContext *ctx)
{
    if (auto *idx = ctx->index_constraint()) {
        for (auto *dr : idx->discrete_range()) {
            if (auto *rdecl = dr->range_decl()) {
                if (auto *erange = rdecl->explicit_range()) {
                    translator.makeRange(erange);
                }
            }
        }
    }
}

} // namespace builder
