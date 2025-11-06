#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "vhdlParser.h"

#include <cstddef>
#include <ranges>
#include <utility>

// Recursion is used for traversing expression trees, which are typically shallow in VHDL parsing.
// NOLINTBEGIN(misc-no-recursion)

namespace builder {

auto Translator::makeIfStatement(vhdlParser::If_statementContext *ctx) -> ast::IfStatement
{
    auto stmt = make<ast::IfStatement>(ctx);

    // Main if branch
    const auto &conditions = ctx->condition();
    const auto &sequences = ctx->sequence_of_statements();

    if (conditions.empty() || sequences.empty()) {
        return stmt;
    }

    stmt.if_branch.condition = makeExpr(conditions[0]->expression());
    stmt.if_branch.body = makeSequenceOfStatements(sequences[0]);

    // elsif branches - number of elsif branches is conditions.size() - 1 (minus the initial if)
    // If there's an else, the last sequence doesn't have a condition
    const std::size_t elsif_count = conditions.size() - 1;
    if (elsif_count > 0) {
        stmt.elsif_branches.reserve(elsif_count);
    }

    for (std::size_t i = 1; i < conditions.size(); ++i) {
        ast::IfStatement::Branch elsif_branch;
        elsif_branch.condition = makeExpr(conditions[i]->expression());
        elsif_branch.body = makeSequenceOfStatements(sequences[i]);
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
    auto stmt = make<ast::CaseStatement>(ctx);

    if (auto *expr = ctx->expression()) {
        stmt.selector = makeExpr(expr);
    }

    const auto &alternatives = ctx->case_statement_alternative();
    stmt.when_clauses.reserve(alternatives.size());

    for (auto *alt : alternatives) {
        ast::CaseStatement::WhenClause when_clause;

        if (auto *choices_ctx = alt->choices()) {
            const auto &choices = choices_ctx->choice();
            when_clause.choices.reserve(choices.size());
            for (auto *ch : choices) {
                when_clause.choices.push_back(makeChoice(ch));
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
    auto loop = make<ast::ForLoop>(ctx);

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
                        // It's a name - cache getText() result
                        loop.range = makeToken(range_decl, range_decl->getText());
                    }
                } else if (auto *subtype = range->subtype_indication()) {
                    // Cache getText() result
                    loop.range = makeToken(subtype, subtype->getText());
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
    auto loop = make<ast::WhileLoop>(ctx);

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
