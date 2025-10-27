#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <cstddef>
#include <utility>

// NOLINTBEGIN(misc-no-recursion)

namespace builder {

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
    const size_t num_elsif = conditions.size() - 1;

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

} // namespace builder

// NOLINTEND(misc-no-recursion)
