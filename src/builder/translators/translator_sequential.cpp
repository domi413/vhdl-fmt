#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <vector>

namespace builder {

auto Translator::makeTarget(vhdlParser::TargetContext *ctx) -> ast::Expr
{
    // Dispatch based on concrete target type
    if (auto *name = ctx->name()) {
        return makeName(name);
    }
    if (auto *agg = ctx->aggregate()) {
        return makeAggregate(agg);
    }

    // Fallback: return token with context text
    ast::TokenExpr token;
    trivia_.bind(token, ctx);
    token.text = ctx->getText();
    return token;
}

auto Translator::makeSequentialAssign(vhdlParser::Signal_assignment_statementContext *ctx)
  -> ast::SequentialAssign
{
    ast::SequentialAssign assign;
    trivia_.bind(assign, ctx);

    if (auto *target_ctx = ctx->target()) {
        assign.target = makeTarget(target_ctx);
    }

    if (auto *wave = ctx->waveform()) {
        auto wave_elems = wave->waveform_element();
        if (!wave_elems.empty() && !wave_elems[0]->expression().empty()) {
            assign.value = makeExpr(wave_elems[0]->expression(0));
        }
    }

    return assign;
}

auto Translator::makeVariableAssign(vhdlParser::Variable_assignment_statementContext *ctx)
  -> ast::SequentialAssign
{
    ast::SequentialAssign assign;
    trivia_.bind(assign, ctx);

    if (auto *target_ctx = ctx->target()) {
        assign.target = makeTarget(target_ctx);
    }

    if (auto *expr = ctx->expression()) {
        assign.value = makeExpr(expr);
    }

    return assign;
}

auto Translator::makeSequentialStatement(vhdlParser::Sequential_statementContext *ctx)
  -> ast::SequentialStatement
{
    // Dispatch based on concrete statement type
    if (auto *signal_assign = ctx->signal_assignment_statement()) {
        return makeSequentialAssign(signal_assign);
    }
    if (auto *var_assign = ctx->variable_assignment_statement()) {
        return makeVariableAssign(var_assign);
    }
    if (auto *if_stmt = ctx->if_statement()) {
        return makeIfStatement(if_stmt);
    }
    if (auto *case_stmt = ctx->case_statement()) {
        return makeCaseStatement(case_stmt);
    }
    if (auto *loop_stmt = ctx->loop_statement()) {
        if (auto *iter = loop_stmt->iteration_scheme()) {
            if (iter->parameter_specification() != nullptr) {
                return makeForLoop(loop_stmt);
            }
            if (iter->condition() != nullptr) {
                return makeWhileLoop(loop_stmt);
            }
        }
        // Basic loop without iteration scheme - not yet supported, return empty
    }

    // TODO(dyb): Add support for wait_statement, assertion_statement,
    // report_statement, next_statement, exit_statement, return_statement, etc.

    // Fallback: return empty assignment as placeholder
    return ast::SequentialAssign{};
}

auto Translator::makeSequenceOfStatements(vhdlParser::Sequence_of_statementsContext *ctx)
  -> std::vector<ast::SequentialStatement>
{
    std::vector<ast::SequentialStatement> statements;

    for (auto *stmt : ctx->sequential_statement()) {
        statements.emplace_back(makeSequentialStatement(stmt));
    }

    return statements;
}

} // namespace builder
