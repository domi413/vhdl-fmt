#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "builder/visitors/sequential_statement_visitor.hpp"
#include "builder/visitors/target_visitor.hpp"
#include "vhdlParser.h"

// NOLINTBEGIN(misc-no-recursion)

namespace builder {

namespace {
auto tryMakeSequentialStatement(Translator &trans, vhdlParser::Sequential_statementContext *stmt)
  -> std::optional<ast::SequentialStatement>
{
    return SequentialStatementVisitor::translate(trans, stmt);
}
} // anonymous namespace

auto Translator::makeConcurrentAssign(
  vhdlParser::Concurrent_signal_assignment_statementContext *ctx) -> ast::ConcurrentAssign
{
    ast::ConcurrentAssign assign;
    trivia_.bind(assign, ctx);

    // Handle conditional_signal_assignment
    if (auto *cond = ctx->conditional_signal_assignment()) {
        if (auto *target_ctx = cond->target()) {
            if (auto target = TargetVisitor::translate(*this, target_ctx)) {
                assign.target = std::move(*target);
            }
        }

        // Get the waveform - for now we'll take the first waveform element's expression
        if (auto *cond_wave = cond->conditional_waveforms()) {
            if (auto *wave = cond_wave->waveform()) {
                auto wave_elems = wave->waveform_element();
                if (!wave_elems.empty() && !wave_elems[0]->expression().empty()) {
                    assign.value = makeExpr(wave_elems[0]->expression(0));
                }
            }
        }
    }
    // Handle selected_signal_assignment
    else if (auto *sel = ctx->selected_signal_assignment()) {
        if (auto *target_ctx = sel->target()) {
            if (auto target = TargetVisitor::translate(*this, target_ctx)) {
                assign.target = std::move(*target);
            }
        }

        // For selected assignments (with...select), we'll take the first waveform
        // TODO(dyb): Handle full selected waveforms structure
        if (auto *sel_waves = sel->selected_waveforms()) {
            auto waves = sel_waves->waveform();
            if (!waves.empty()) {
                auto wave_elems = waves[0]->waveform_element();
                if (!wave_elems.empty() && !wave_elems[0]->expression().empty()) {
                    assign.value = makeExpr(wave_elems[0]->expression(0));
                }
            }
        }
    }

    return assign;
}

auto Translator::makeSequentialAssign(vhdlParser::Signal_assignment_statementContext *ctx)
  -> ast::SequentialAssign
{
    ast::SequentialAssign assign;
    trivia_.bind(assign, ctx);

    if (auto *target_ctx = ctx->target()) {
        if (auto target = TargetVisitor::translate(*this, target_ctx)) {
            assign.target = std::move(*target);
        }
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
        if (auto target = TargetVisitor::translate(*this, target_ctx)) {
            assign.target = std::move(*target);
        }
    }

    if (auto *expr = ctx->expression()) {
        assign.value = makeExpr(expr);
    }

    return assign;
}

auto Translator::makeIfStatement(vhdlParser::If_statementContext *ctx) -> ast::IfStatement
{
    ast::IfStatement stmt;
    trivia_.bind(stmt, ctx);

    // Main if branch
    auto conditions = ctx->condition();
    auto sequences = ctx->sequence_of_statements();

    if (!conditions.empty() && !sequences.empty()) {
        stmt.if_branch.condition = makeExpr(conditions[0]->expression());
        stmt.if_branch.body = makeSequenceOfStatements(sequences[0]);
    }

    // elsif branches - number of elsif branches is conditions.size() - 1 (minus the initial if)
    // If there's an else, the last sequence doesn't have a condition
    size_t num_elsif = conditions.size() - 1;

    for (size_t i = 0; i < num_elsif; ++i) {
        ast::IfStatement::Branch elsif_branch;
        elsif_branch.condition = makeExpr(conditions[i + 1]->expression());
        elsif_branch.body = makeSequenceOfStatements(sequences[i + 1]);
        stmt.elsif_branches.push_back(std::move(elsif_branch));
    }

    // else branch - if there are more sequences than conditions, the last one is else
    if (sequences.size() > conditions.size()) {
        ast::IfStatement::Branch else_branch;
        // else has no condition - leave it empty
        else_branch.body = makeSequenceOfStatements(sequences.back());
        stmt.else_branch = std::move(else_branch);
    }

    return stmt;
}

auto Translator::makeCaseStatement(vhdlParser::Case_statementContext *ctx) -> ast::CaseStatement
{
    ast::CaseStatement stmt;
    trivia_.bind(stmt, ctx);

    if (auto *expr = ctx->expression()) {
        stmt.selector = makeExpr(expr);
    }

    for (auto *alt : ctx->case_statement_alternative()) {
        ast::CaseStatement::WhenClause when_clause;

        if (auto *choices_ctx = alt->choices()) {
            for (auto *choice : choices_ctx->choice()) {
                when_clause.choices.push_back(makeChoice(choice));
            }
        }

        if (auto *seq = alt->sequence_of_statements()) {
            when_clause.body = makeSequenceOfStatements(seq);
        }

        stmt.when_clauses.push_back(std::move(when_clause));
    }

    return stmt;
}

auto Translator::makeProcess(vhdlParser::Process_statementContext *ctx) -> ast::Process
{
    ast::Process proc;
    trivia_.bind(proc, ctx);

    // Extract label if present
    if (auto *label = ctx->label_colon()) {
        if (auto *id = label->identifier()) {
            proc.label = id->getText();
        }
    }

    // Extract sensitivity list
    if (auto *sens_list = ctx->sensitivity_list()) {
        for (auto *name_ctx : sens_list->name()) {
            proc.sensitivity_list.push_back(name_ctx->getText());
        }
    }

    // Extract sequential statements
    if (auto *stmt_part = ctx->process_statement_part()) {
        for (auto *stmt : stmt_part->sequential_statement()) {
            if (auto result = tryMakeSequentialStatement(*this, stmt)) {
                proc.body.emplace_back(std::move(*result));
            }
        }
    }

    return proc;
}

auto Translator::makeForLoop(vhdlParser::Loop_statementContext *ctx) -> ast::ForLoop
{
    ast::ForLoop loop;
    trivia_.bind(loop, ctx);

    // Check if it has a FOR iteration scheme
    if (auto *iter = ctx->iteration_scheme()) {
        if (auto *param = iter->parameter_specification()) {
            if (auto *id = param->identifier()) {
                loop.iterator = id->getText();
            }

            if (auto *range = param->discrete_range()) {
                // discrete_range can be range_decl or subtype_indication
                if (auto *range_decl = range->range_decl()) {
                    if (auto *explicit_r = range_decl->explicit_range()) {
                        loop.range = makeRange(explicit_r);
                    } else {
                        // It's a name
                        ast::TokenExpr tok;
                        trivia_.bind(tok, range_decl);
                        tok.text = range_decl->getText();
                        loop.range = tok;
                    }
                } else if (auto *subtype = range->subtype_indication()) {
                    ast::TokenExpr tok;
                    trivia_.bind(tok, subtype);
                    tok.text = subtype->getText();
                    loop.range = tok;
                }
            }
        }
    }

    if (auto *seq = ctx->sequence_of_statements()) {
        loop.body = makeSequenceOfStatements(seq);
    }

    return loop;
}

auto Translator::makeWhileLoop(vhdlParser::Loop_statementContext *ctx) -> ast::WhileLoop
{
    ast::WhileLoop loop;
    trivia_.bind(loop, ctx);

    // Check if it has a WHILE iteration scheme
    if (auto *iter = ctx->iteration_scheme()) {
        if (auto *cond = iter->condition()) {
            loop.condition = makeExpr(cond->expression());
        }
    }

    if (auto *seq = ctx->sequence_of_statements()) {
        loop.body = makeSequenceOfStatements(seq);
    }

    return loop;
}

auto Translator::makeSequenceOfStatements(vhdlParser::Sequence_of_statementsContext *ctx)
  -> std::vector<ast::SequentialStatement>
{
    std::vector<ast::SequentialStatement> statements;

    for (auto *stmt : ctx->sequential_statement()) {
        if (auto result = tryMakeSequentialStatement(*this, stmt)) {
            statements.emplace_back(std::move(*result));
        }
    }

    return statements;
}

} // namespace builder

// NOLINTEND(misc-no-recursion)
