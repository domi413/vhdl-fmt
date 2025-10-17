#include "builder/visitor.hpp"

#include "ast/nodes/entity.hpp"
#include "vhdlParser.h"

namespace builder {

// -----------------------------------------------------------------------------
// Entity / Architecture
// -----------------------------------------------------------------------------
void Visitor::visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx)
{
    auto &entity = translator_.makeEntity(ctx);

    if (auto *header = ctx->entity_header()) {
        if (auto *gen_clause = header->generic_clause()) {
            translator_.into(entity.generic_clause, [&] { dispatch(gen_clause); });
        }
        if (auto *port_clause = header->port_clause()) {
            translator_.into(entity.port_clause, [&] { dispatch(port_clause); });
        }
    }
}

void Visitor::visitArchitectureBody(vhdlParser::Architecture_bodyContext *ctx)
{
    auto &arch = translator_.makeArchitecture(ctx);

    if (auto *decl_part = ctx->architecture_declarative_part()) {
        translator_.into(arch.decls, [&] { dispatch(decl_part); });
    }

    if (auto *stmt_part = ctx->architecture_statement_part()) {
        translator_.into(arch.stmts, [&] { dispatch(stmt_part); });
    }
}

// -----------------------------------------------------------------------------
// Clauses
// -----------------------------------------------------------------------------
void Visitor::visitGenericClause(vhdlParser::Generic_clauseContext *ctx)
{
    auto &clause = translator_.makeGenericClause(ctx);

    if (auto *list = ctx->generic_list()) {
        translator_.into(clause.generics, [&] {
            for (auto *decl : list->interface_constant_declaration()) {
                auto &param = translator_.makeGenericParam(decl);

                if (auto *expr = decl->expression()) {
                    translator_.into(param.default_expr, [&] { dispatch(expr); });
                }
            }
        });
    }
}

void Visitor::visitPortClause(vhdlParser::Port_clauseContext *ctx)
{
    auto &clause = translator_.makePortClause(ctx);

    auto *list = ctx->port_list();
    if (list == nullptr) {
        return;
    }
    auto *iface = list->interface_port_list();
    if (iface == nullptr) {
        return;
    }
    translator_.into(clause.ports, [&] {
        for (auto *decl : iface->interface_port_declaration()) {
            auto &port = translator_.makeSignalPort(decl);

            if (auto *expr = decl->expression()) {
                translator_.into(port.default_expr, [&] { dispatch(expr); });
            }

            if (auto *stype = decl->subtype_indication()) {
                if (auto *constraint = stype->constraint()) {
                    translator_.into(port.constraints, [&] { dispatch(constraint); });
                }
            }
        }
    });
}

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------
void Visitor::visitConstantDeclaration(vhdlParser::Constant_declarationContext *ctx)
{
    translator_.makeConstantDecl(ctx);
}

void Visitor::visitSignalDeclaration(vhdlParser::Signal_declarationContext *ctx)
{
    auto &signal = translator_.makeSignalDecl(ctx);
    translator_.into(signal.constraints, [&] {
        if (auto *stype = ctx->subtype_indication()) {
            if (auto *constraint = stype->constraint()) {
                dispatch(constraint);
            }
        }
    });
}

// -----------------------------------------------------------------------------
// Constraints
// -----------------------------------------------------------------------------
void Visitor::visitConstraint(vhdlParser::ConstraintContext *ctx)
{
    auto *index_con = ctx->index_constraint();
    if (index_con == nullptr) {
        return;
    }
    for (auto *range_ctx : index_con->discrete_range()) {
        if (auto *range_decl = range_ctx->range_decl()) {
            if (auto *explicit_range = range_decl->explicit_range()) {
                auto &range = translator_.makeRange(explicit_range);

                translator_.into(range.left, [&] {
                    if (auto *left = explicit_range->simple_expression(0)) {
                        dispatch(left);
                    }
                });
                translator_.into(range.right, [&] {
                    if (auto *right = explicit_range->simple_expression(1)) {
                        dispatch(right);
                    }
                });
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Expressions
// -----------------------------------------------------------------------------
void Visitor::visitSimpleExpression(vhdlParser::Simple_expressionContext *ctx)
{
    translator_.makeExpr(ctx);
}

} // namespace builder
