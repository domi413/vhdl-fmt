#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <optional>
#include <ranges>
#include <utility>
#include <vector>

namespace builder {

auto Translator::tryMakeExpr(vhdlParser::ExpressionContext *ctx) -> std::optional<ast::Expr>
{
    if (ctx == nullptr) {
        return std::nullopt;
    }

    return makeExpr(*ctx);
}

auto Translator::makeTarget(vhdlParser::TargetContext &ctx) -> ast::Expr
{
    // Dispatch based on concrete target type
    if (auto *name = ctx.name()) {
        return makeName(*name);
    }
    if (auto *agg = ctx.aggregate()) {
        return makeAggregate(*agg);
    }

    // Fallback: return token with context text
    auto token = make<ast::TokenExpr>(ctx);
    token.text = ctx.getText();
    return token;
}

auto Translator::makeSequentialAssign(vhdlParser::Signal_assignment_statementContext &ctx)
  -> ast::SequentialAssign
{
    auto assign = make<ast::SequentialAssign>(ctx);

    if (auto *target_ctx = ctx.target()) {
        assign.target = makeTarget(*target_ctx);
    }

    if (auto *wave = ctx.waveform()) {
        auto wave_elems = wave->waveform_element();
        if (!wave_elems.empty() && !wave_elems[0]->expression().empty()) {
            assign.value = makeExpr(*wave_elems[0]->expression(0));
        }
    }

    return assign;
}

auto Translator::makeVariableAssign(vhdlParser::Variable_assignment_statementContext &ctx)
  -> ast::SequentialAssign
{
    auto assign = make<ast::SequentialAssign>(ctx);

    if (auto *target_ctx = ctx.target()) {
        assign.target = makeTarget(*target_ctx);
    }

    if (auto *expr = ctx.expression()) {
        assign.value = makeExpr(*expr);
    }

    return assign;
}

auto Translator::makeSequentialStatement(vhdlParser::Sequential_statementContext &ctx)
  -> ast::SequentialStatement
{
    // Dispatch based on concrete statement type
    if (auto *signal_assign = ctx.signal_assignment_statement()) {
        return makeSequentialAssign(*signal_assign);
    }
    if (auto *var_assign = ctx.variable_assignment_statement()) {
        return makeVariableAssign(*var_assign);
    }
    if (auto *if_stmt = ctx.if_statement()) {
        return makeIfStatement(*if_stmt);
    }
    if (auto *case_stmt = ctx.case_statement()) {
        return makeCaseStatement(*case_stmt);
    }
    if (auto *loop_stmt = ctx.loop_statement()) {
        if (auto *iter = loop_stmt->iteration_scheme()) {
            if (iter->parameter_specification() != nullptr) {
                return makeForLoop(*loop_stmt);
            }
            if (iter->condition() != nullptr) {
                return makeWhileLoop(*loop_stmt);
            }
        }
        // Basic loop without iteration scheme - not yet supported
    }
    if (ctx.NULL_() != nullptr) {
        return make<ast::NullStatement>(ctx);
    }
    if (auto *wait_stmt = ctx.wait_statement()) {
        return makeWaitStatement(*wait_stmt);
    }
    if (auto *return_stmt = ctx.return_statement()) {
        return makeReturnStatement(*return_stmt);
    }
    if (auto *next_stmt = ctx.next_statement()) {
        return makeNextStatement(*next_stmt);
    }
    if (auto *exit_stmt = ctx.exit_statement()) {
        return makeExitStatement(*exit_stmt);
    }
    if (auto *report_stmt = ctx.report_statement()) {
        return makeReportStatement(*report_stmt);
    }
    if (auto *assert_stmt = ctx.assertion_statement()) {
        return makeAssertStatement(*assert_stmt);
    }
    if (auto *break_stmt = ctx.break_statement()) {
        return makeBreakStatement(*break_stmt);
    }
    if (auto *proc_call = ctx.procedure_call_statement()) {
        return makeProcedureCall(*proc_call);
    }

    // Fallback: return null statement as placeholder
    return make<ast::NullStatement>(ctx);
}

auto Translator::makeSequenceOfStatements(vhdlParser::Sequence_of_statementsContext &ctx)
  -> std::vector<ast::SequentialStatement>
{
    std::vector<ast::SequentialStatement> statements;

    for (auto *stmt : ctx.sequential_statement()) {
        statements.emplace_back(makeSequentialStatement(*stmt));
    }

    return statements;
}

// ---------------------- Simple Sequential Statements ----------------------

auto Translator::makeWaitStatement(vhdlParser::Wait_statementContext &ctx) -> ast::WaitStatement
{
    auto stmt = make<ast::WaitStatement>(ctx);

    // Extract condition (wait until condition)
    if (auto *cond_clause = ctx.condition_clause()) {
        if (auto *cond = cond_clause->condition()) {
            stmt.condition = tryMakeExpr(cond->expression());
        }
    }

    // Extract sensitivity list (wait on signals)
    if (auto *sens = ctx.sensitivity_clause()) {
        if (auto *list = sens->sensitivity_list()) {
            stmt.sensitivity_list = list->name()
                                  | std::views::transform([](auto *n) { return n->getText(); })
                                  | std::ranges::to<std::vector>();
        }
    }

    // Extract timeout (wait for duration)
    if (auto *timeout_clause = ctx.timeout_clause()) {
        stmt.timeout = tryMakeExpr(timeout_clause->expression());
    }

    return stmt;
}

auto Translator::makeReturnStatement(vhdlParser::Return_statementContext &ctx)
  -> ast::ReturnStatement
{
    auto stmt = make<ast::ReturnStatement>(ctx);
    stmt.value = tryMakeExpr(ctx.expression());
    return stmt;
}

auto Translator::makeNextStatement(vhdlParser::Next_statementContext &ctx) -> ast::NextStatement
{
    auto stmt = make<ast::NextStatement>(ctx);

    if (auto *id = ctx.identifier()) {
        stmt.loop_label = id->getText();
    }

    if (auto *cond = ctx.condition()) {
        stmt.condition = tryMakeExpr(cond->expression());
    }

    return stmt;
}

auto Translator::makeExitStatement(vhdlParser::Exit_statementContext &ctx) -> ast::ExitStatement
{
    auto stmt = make<ast::ExitStatement>(ctx);

    if (auto *id = ctx.identifier()) {
        stmt.loop_label = id->getText();
    }

    if (auto *cond = ctx.condition()) {
        stmt.condition = tryMakeExpr(cond->expression());
    }

    return stmt;
}

auto Translator::makeReportStatement(vhdlParser::Report_statementContext &ctx)
  -> ast::ReportStatement
{
    auto stmt = make<ast::ReportStatement>(ctx);

    if (auto *expr = ctx.expression(0)) {
        stmt.message = makeExpr(*expr);
    }

    // Severity is the second expression if present
    if (ctx.expression().size() > 1) {
        stmt.severity = makeExpr(*ctx.expression(1));
    }

    return stmt;
}

auto Translator::makeAssertStatement(vhdlParser::Assertion_statementContext &ctx)
  -> ast::AssertStatement
{
    auto stmt = make<ast::AssertStatement>(ctx);

    auto *assertion = ctx.assertion();
    if (assertion == nullptr) {
        return stmt;
    }

    // Extract condition
    if (auto *cond = assertion->condition()) {
        if (auto *expr = cond->expression()) {
            stmt.condition = makeExpr(*expr);
        }
    }

    const auto &expressions = assertion->expression();

    // Extract report message (REPORT keyword followed by expression)
    if (assertion->REPORT() != nullptr && !expressions.empty()) {
        stmt.message = makeExpr(*expressions[0]);
    }

    // Extract severity level (SEVERITY keyword followed by expression)
    if (assertion->SEVERITY() != nullptr && expressions.size() > 1) {
        stmt.severity = makeExpr(*expressions[1]);
    }

    return stmt;
}

auto Translator::makeBreakStatement(vhdlParser::Break_statementContext &ctx) -> ast::BreakStatement
{
    auto stmt = make<ast::BreakStatement>(ctx);

    // Extract break elements (quantity => expression pairs)
    if (auto *break_list = ctx.break_list()) {
        auto make_element = [this](auto *elem) {
            auto assoc = make<ast::BinaryExpr>(*elem);
            assoc.op = "=>";

            if (auto *name = elem->name()) {
                assoc.left = std::make_unique<ast::Expr>(makeName(*name));
            }

            if (auto *expr = elem->expression()) {
                assoc.right = std::make_unique<ast::Expr>(makeExpr(*expr));
            }

            return ast::Expr{ std::move(assoc) };
        };

        stmt.break_elements = break_list->break_element()
                            | std::views::transform(make_element)
                            | std::ranges::to<std::vector>();
    }

    // Extract condition (when clause)
    if (auto *cond = ctx.condition()) {
        stmt.condition = tryMakeExpr(cond->expression());
    }

    return stmt;
}

auto Translator::makeProcedureCall(vhdlParser::Procedure_call_statementContext &ctx)
  -> ast::ProcedureCall
{
    auto stmt = make<ast::ProcedureCall>(ctx);

    if (auto *proc_call = ctx.procedure_call()) {
        if (auto *name = proc_call->selected_name()) {
            stmt.call = makeToken(name, name->getText());
        }
    }

    return stmt;
}

} // namespace builder
