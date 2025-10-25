#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

namespace builder {

// ---------------------- Top-level ----------------------

void Translator::buildDesignFile(ast::DesignFile &dest, vhdlParser::Design_fileContext *ctx)
{
    for (auto *unit_ctx : ctx->design_unit()) {
        auto *lib_unit = unit_ctx->library_unit();
        if (lib_unit == nullptr) {
            continue;
        }

        // Check primary units (entity_declaration | configuration_declaration |
        // package_declaration)
        if (auto *primary = lib_unit->primary_unit()) {
            if (auto *entity_ctx = primary->entity_declaration()) {
                dest.units.emplace_back(makeEntity(entity_ctx));
            }
            // TODO(translator): Handle configuration_declaration and package_declaration
        }
        // Check secondary units (architecture_body | package_body)
        else if (auto *secondary = lib_unit->secondary_unit()) {
            if (auto *arch_ctx = secondary->architecture_body()) {
                dest.units.emplace_back(makeArchitecture(arch_ctx));
            }
            // TODO(translator): Handle package_body
        }
    }
}

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

    return entity;
}

auto Translator::makeArchitecture(vhdlParser::Architecture_bodyContext *ctx) -> ast::Architecture
{
    ast::Architecture arch;
    trivia_.bind(arch, ctx);

    arch.name = ctx->identifier(0)->getText();
    arch.entity_name = ctx->identifier(1)->getText();

    // Walk declarative part and collect declarations directly
    if (auto *decl_part = ctx->architecture_declarative_part()) {
        for (auto *item : decl_part->block_declarative_item()) {
            if (auto *const_ctx = item->constant_declaration()) {
                arch.decls.emplace_back(makeConstantDecl(const_ctx));
            } else if (auto *sig_ctx = item->signal_declaration()) {
                arch.decls.emplace_back(makeSignalDecl(sig_ctx));
            }
            // TODO(user): Add more declaration types as needed (variables, types, subprograms,
            // etc.)
        }
    }

    // Walk statement part and collect concurrent statements
    if (auto *stmt_part = ctx->architecture_statement_part()) {
        for (auto *stmt : stmt_part->architecture_statement()) {
            if (auto *proc = stmt->process_statement()) {
                arch.stmts.emplace_back(makeProcess(proc));
            } else if (auto *sig_assign = stmt->concurrent_signal_assignment_statement()) {
                arch.stmts.emplace_back(makeConcurrentAssign(sig_assign));
            }
            // TODO(user): Add more concurrent statement types (component instantiation, generate,
            // etc.)
        }
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
