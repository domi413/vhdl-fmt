#include "ast/nodes/design_units.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

namespace builder {

// ---------------------- Design units ----------------------

auto Translator::makeEntity(vhdlParser::Entity_declarationContext *ctx) -> ast::Entity
{
    ast::Entity entity;
    trivia_.bind(entity, ctx);
    
    entity.name = ctx->identifier(0)->getText();

    // Optional end label (ENTITY ... END ENTITY <id>)
    if (ctx->identifier().size() > 1) {
        entity.end_label = ctx->identifier(1)->getText();
    }

    if (auto *header = ctx->entity_header()) {
        if (auto *gen_clause = header->generic_clause()) {
            entity.generic_clause = makeGenericClause(gen_clause);
        }
        if (auto *port_clause = header->port_clause()) {
            entity.port_clause = makePortClause(port_clause);
        }
    }
    
    // Push to destination if set
    if (units_ != nullptr) {
        units_->emplace_back(std::move(entity));
    }
    
    return entity;
}

auto Translator::makeArchitecture(vhdlParser::Architecture_bodyContext *ctx) -> ast::Architecture
{
    ast::Architecture arch;
    trivia_.bind(arch, ctx);
    
    arch.name = ctx->identifier(0)->getText();
    arch.entity_name = ctx->identifier(1)->getText();

    if (auto *decl_part = ctx->architecture_declarative_part()) {
        setDeclsDestination(arch.decls);
        walk_(decl_part);
        clearDeclsDestination();
    }

    if (auto *stmt_part = ctx->architecture_statement_part()) {
        setStmtsDestination(arch.stmts);
        walk_(stmt_part);
        clearStmtsDestination();
    }
    
    // Push to destination if set
    if (units_ != nullptr) {
        units_->push_back(std::move(arch));
    }
    
    return arch;
}

// ------------------------ Clauses -------------------------

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

} // namespace builder
