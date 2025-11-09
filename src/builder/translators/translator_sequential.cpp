#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <ranges>
#include <vector>

namespace builder {

auto Translator::makeTarget(vhdlParser::TargetContext *ctx) -> ast::Expression
{
    // Dispatch based on concrete target type
    if (auto *name = ctx->name()) {
        return makeName(name);
    }

    if (auto *agg = ctx->aggregate()) {
        return makeAggregate(agg);
    }

    // Fallback: return token with context text
    auto token = make<ast::TokenExpr>(ctx);
    token.text = ctx->getText();
    return token;
}

auto Translator::makeSignalAssignmentStatement(vhdlParser::Signal_assignment_statementContext *ctx)
  -> ast::SignalAssignmentStatement
{
    auto assign = make<ast::SignalAssignmentStatement>(ctx);

    if (auto *target_ctx = ctx->target()) {
        assign.target = makeTarget(target_ctx);
    }

    if (auto *wave = ctx->waveform()) {
        auto wave_elems = wave->waveform_element();
        if (!wave_elems.empty() && !wave_elems[0]->expression().empty()) {
            assign.value = makeExpression(wave_elems[0]->expression(0));
        }
    }

    return assign;
}

auto Translator::makeVariableAssignStatement(vhdlParser::Variable_assignment_statementContext *ctx)
  -> ast::SignalAssignmentStatement
{
    auto assign = make<ast::SignalAssignmentStatement>(ctx);

    if (auto *target_ctx = ctx->target()) {
        assign.target = makeTarget(target_ctx);
    }

    if (auto *expr = ctx->expression()) {
        assign.value = makeExpression(expr);
    }

    return assign;
}

auto Translator::makeSequentialStatement(vhdlParser::Sequential_statementContext *ctx)
  -> ast::SequentialStatement
{
    // Dispatch based on concrete statement type
    if (auto *signal_assign = ctx->signal_assignment_statement()) {
        return makeSignalAssignmentStatement(signal_assign);
    }
    if (auto *var_assign = ctx->variable_assignment_statement()) {
        return makeVariableAssignStatement(var_assign);
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
                return makeForLoopStatement(loop_stmt);
            }
            if (iter->condition() != nullptr) {
                return makeWhileLoopStatement(loop_stmt);
            }
        }
        // Basic loop without iteration scheme - not yet supported, return empty
    }
    if (auto *wait_stmt = ctx->wait_statement()) {
        return makeWaitStatement(wait_stmt);
    }
    if (auto *assert_stmt = ctx->assertion_statement()) {
        return makeAssertStatement(assert_stmt);
    }
    if (auto *report_stmt = ctx->report_statement()) {
        return makeReportStatement(report_stmt);
    }
    if (auto *next_stmt = ctx->next_statement()) {
        return makeNextStatement(next_stmt);
    }
    if (auto *exit_stmt = ctx->exit_statement()) {
        return makeExitStatement(exit_stmt);
    }
    if (auto *return_stmt = ctx->return_statement()) {
        return makeReturnStatement(return_stmt);
    }
    if (auto *proc_call = ctx->procedure_call_statement()) {
        return makeProcedureCallStatement(proc_call);
    }
    if (ctx->NULL_() != nullptr) {
        return makeNullStatement(ctx);
    }

    // Fallback: return empty null statement as placeholder
    return ast::NullStatement{};
}

auto Translator::makeSequenceOfStatements(vhdlParser::Sequence_of_statementsContext *ctx)
  -> std::vector<ast::SequentialStatement>
{
    std::vector<ast::SequentialStatement> statements{};

    for (auto *stmt : ctx->sequential_statement()) {
        statements.emplace_back(makeSequentialStatement(stmt));
    }

    return statements;
}

auto Translator::makeWaitStatement(vhdlParser::Wait_statementContext *ctx) -> ast::WaitStatement
{
    auto stmt = make<ast::WaitStatement>(ctx);

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            stmt.label = id->getText();
        }
    }

    // Extract sensitivity clause (WAIT ON signal_list)
    if (auto *sens = ctx->sensitivity_clause()) {
        if (auto *sens_list = sens->sensitivity_list()) {
            stmt.sensitivity_list
              = sens_list->name()
              | std::views::transform([](auto *name) { return name->getText(); })
              | std::ranges::to<std::vector>();
        }
    }

    // Extract condition clause (WAIT UNTIL condition)
    if (auto *cond = ctx->condition_clause()) {
        if (auto *condition = cond->condition()) {
            if (auto *expr = condition->expression()) {
                stmt.condition = makeExpression(expr);
            }
        }
    }

    // Extract timeout clause (WAIT FOR time_expression)
    if (auto *timeout = ctx->timeout_clause()) {
        if (auto *expr = timeout->expression()) {
            stmt.timeout = makeExpression(expr);
        }
    }

    return stmt;
}

auto Translator::makeAssertStatement(vhdlParser::Assertion_statementContext *ctx)
  -> ast::AssertStatement
{
    auto stmt = make<ast::AssertStatement>(ctx);

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            stmt.label = id->getText();
        }
    }

    if (auto *assertion = ctx->assertion()) {
        // Extract condition
        if (auto *condition = assertion->condition()) {
            if (auto *expr = condition->expression()) {
                stmt.condition = makeExpression(expr);
            }
        }

        // Extract report and severity expressions
        auto exprs = assertion->expression();
        if (!exprs.empty()) {
            stmt.report_expr = makeExpression(exprs[0]);
        }
        if (exprs.size() > 1) {
            stmt.severity_expr = makeExpression(exprs[1]);
        }
    }

    return stmt;
}

auto Translator::makeReportStatement(vhdlParser::Report_statementContext *ctx)
  -> ast::ReportStatement
{
    auto stmt = make<ast::ReportStatement>(ctx);

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            stmt.label = id->getText();
        }
    }

    // Extract message and severity expressions
    auto exprs = ctx->expression();
    if (!exprs.empty()) {
        stmt.message = makeExpression(exprs[0]);
    }
    if (exprs.size() > 1) {
        stmt.severity_expr = makeExpression(exprs[1]);
    }

    return stmt;
}

auto Translator::makeNextStatement(vhdlParser::Next_statementContext *ctx) -> ast::NextStatement
{
    auto stmt = make<ast::NextStatement>(ctx);

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            stmt.label = id->getText();
        }
    }

    // Extract loop label (which loop to continue)
    if (auto *loop_id = ctx->identifier()) {
        stmt.loop_label = loop_id->getText();
    }

    // Extract condition (WHEN condition)
    if (auto *condition = ctx->condition()) {
        if (auto *expr = condition->expression()) {
            stmt.condition = makeExpression(expr);
        }
    }

    return stmt;
}

auto Translator::makeExitStatement(vhdlParser::Exit_statementContext *ctx) -> ast::ExitStatement
{
    auto stmt = make<ast::ExitStatement>(ctx);

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            stmt.label = id->getText();
        }
    }

    // Extract loop label (which loop to exit)
    if (auto *loop_id = ctx->identifier()) {
        stmt.loop_label = loop_id->getText();
    }

    // Extract condition (WHEN condition)
    if (auto *condition = ctx->condition()) {
        if (auto *expr = condition->expression()) {
            stmt.condition = makeExpression(expr);
        }
    }

    return stmt;
}

auto Translator::makeReturnStatement(vhdlParser::Return_statementContext *ctx)
  -> ast::ReturnStatement
{
    auto stmt = make<ast::ReturnStatement>(ctx);

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            stmt.label = id->getText();
        }
    }

    // Extract return value expression
    if (auto *expr = ctx->expression()) {
        stmt.value = makeExpression(expr);
    }

    return stmt;
}

auto Translator::makeProcedureCallStatement(vhdlParser::Procedure_call_statementContext *ctx)
  -> ast::ProcedureCallStatement
{
    auto stmt = make<ast::ProcedureCallStatement>(ctx);

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            stmt.label = id->getText();
        }
    }

    if (auto *proc_call = ctx->procedure_call()) {
        // Extract procedure name
        if (auto *selected = proc_call->selected_name()) {
            stmt.procedure_name = selected->getText();
        }

        // Extract arguments if present
        if (auto *params = proc_call->actual_parameter_part()) {
            if (auto *assoc_list = params->association_list()) {
                auto associations = assoc_list->association_element();
                if (associations.size() == 1) {
                    stmt.args = makeAssociationElement(associations[0]);
                } else if (associations.size() > 1) {
                    auto group = make<ast::GroupExpr>(assoc_list);
                    for (auto *elem : associations) {
                        group.children.emplace_back(makeAssociationElement(elem));
                    }
                    stmt.args = std::move(group);
                }
            }
        }
    }

    return stmt;
}

auto Translator::makeNullStatement(vhdlParser::Sequential_statementContext *ctx)
  -> ast::NullStatement
{
    auto stmt = make<ast::NullStatement>(ctx);

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            stmt.label = id->getText();
        }
    }

    return stmt;
}

} // namespace builder
