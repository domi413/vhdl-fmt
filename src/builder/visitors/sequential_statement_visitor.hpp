#pragma once

#include "ast/nodes/statements.hpp"
#include "builder/translator.hpp"
#include "builder/typed_visitor.hpp"

namespace builder {

// CRTP-based visitor for sequential statements with full type safety
//
// This visitor leverages the TypedVisitor CRTP base to provide:
// - Type-safe returns (no std::any in public API)
// - Clean dispatch through ANTLR's virtual method dispatch
// - No if-else chains
//
// Usage:
//   auto stmt = SequentialStatementVisitor::translate(translator, ctx);
//
// The visitor automatically dispatches to the correct visitXXX() method
// based on the parse tree node type, and each method returns the proper
// AST statement type directly.
class SequentialStatementVisitor
  : public TypedVisitor<SequentialStatementVisitor, ast::SequentialStatement>
{
    Translator &trans_;

  public:
    explicit SequentialStatementVisitor(Translator &trans) : trans_(trans) {}

    // Static factory method for convenient usage
    static auto translate(Translator &trans, vhdlParser::Sequential_statementContext *ctx)
      -> std::optional<ast::SequentialStatement>
    {
        SequentialStatementVisitor visitor{ trans };
        return visitor.TypedVisitor::translate(ctx);
    }

  private:
    friend class TypedVisitor<SequentialStatementVisitor, ast::SequentialStatement>;

    // Override ANTLR's generated visit methods for each statement type
    // Each method is type-safe and sets the result directly
    std::any visitSignal_assignment_statement(
      vhdlParser::Signal_assignment_statementContext *ctx) override
    {
        setResult(trans_.makeSequentialAssign(ctx));
        return {};
    }

    std::any visitVariable_assignment_statement(
      vhdlParser::Variable_assignment_statementContext *ctx) override
    {
        setResult(trans_.makeVariableAssign(ctx));
        return {};
    }

    std::any visitIf_statement(vhdlParser::If_statementContext *ctx) override
    {
        setResult(trans_.makeIfStatement(ctx));
        return {};
    }

    std::any visitCase_statement(vhdlParser::Case_statementContext *ctx) override
    {
        setResult(trans_.makeCaseStatement(ctx));
        return {};
    }

    std::any visitLoop_statement(vhdlParser::Loop_statementContext *ctx) override
    {
        if (auto *iter = ctx->iteration_scheme()) {
            if (iter->parameter_specification() != nullptr) {
                setResult(trans_.makeForLoop(ctx));
            } else if (iter->condition() != nullptr) {
                setResult(trans_.makeWhileLoop(ctx));
            }
        }
        // Basic loop without iteration scheme - skip for now
        return {};
    }

    // TODO(dyb): Add overrides for wait_statement, assertion_statement,
    // report_statement, next_statement, exit_statement, return_statement, etc.
};

} // namespace builder
