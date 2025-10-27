#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/expr_helpers.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "vhdlParser.h"

#include <CommonTokenStream.h>
#include <vector>

namespace builder {

class Translator final
{
    TriviaBinder &trivia_;
    antlr4::CommonTokenStream &tokens_;

  public:
    Translator(TriviaBinder &tv, antlr4::CommonTokenStream &tokens) : trivia_(tv), tokens_(tokens)
    {
    }

    /// @brief Build the entire design file by walking the CST
    void buildDesignFile(ast::DesignFile &dest, vhdlParser::Design_fileContext *ctx);

    ~Translator() = default;

    Translator(const Translator &) = delete;
    auto operator=(const Translator &) -> Translator & = delete;
    Translator(Translator &&) = delete;
    auto operator=(Translator &&) -> Translator & = delete;

    // Design units - return by value
    [[nodiscard]]
    auto makeEntity(vhdlParser::Entity_declarationContext *ctx) -> ast::Entity;
    [[nodiscard]]
    auto makeArchitecture(vhdlParser::Architecture_bodyContext *ctx) -> ast::Architecture;

    // Clauses - return by value
    [[nodiscard]]
    auto makeGenericClause(vhdlParser::Generic_clauseContext *ctx) -> ast::GenericClause;
    [[nodiscard]]
    auto makePortClause(vhdlParser::Port_clauseContext *ctx) -> ast::PortClause;

    // Declarations - return by value
    [[nodiscard]]
    auto makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
      -> ast::GenericParam;
    [[nodiscard]]
    auto makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx) -> ast::Port;
    [[nodiscard]]
    auto makeConstantDecl(vhdlParser::Constant_declarationContext *ctx) -> ast::ConstantDecl;
    [[nodiscard]]
    auto makeSignalDecl(vhdlParser::Signal_declarationContext *ctx) -> ast::SignalDecl;

    // Statements - return by value
    [[nodiscard]]
    auto makeConcurrentAssign(vhdlParser::Concurrent_signal_assignment_statementContext *ctx)
      -> ast::ConcurrentAssign;
    [[nodiscard]]
    auto makeSequentialAssign(vhdlParser::Signal_assignment_statementContext *ctx)
      -> ast::SequentialAssign;
    [[nodiscard]]
    auto makeVariableAssign(vhdlParser::Variable_assignment_statementContext *ctx)
      -> ast::SequentialAssign;
    [[nodiscard]]
    auto makeIfStatement(vhdlParser::If_statementContext *ctx) -> ast::IfStatement;
    [[nodiscard]]
    auto makeCaseStatement(vhdlParser::Case_statementContext *ctx) -> ast::CaseStatement;
    [[nodiscard]]
    auto makeProcess(vhdlParser::Process_statementContext *ctx) -> ast::Process;
    [[nodiscard]]
    auto makeForLoop(vhdlParser::Loop_statementContext *ctx) -> ast::ForLoop;
    [[nodiscard]]
    auto makeWhileLoop(vhdlParser::Loop_statementContext *ctx) -> ast::WhileLoop;
    [[nodiscard]]
    auto makeSequenceOfStatements(vhdlParser::Sequence_of_statementsContext *ctx)
      -> std::vector<ast::SequentialStatement>;

    // Expressions - return by value
    [[nodiscard]]
    auto makeExpr(vhdlParser::ExpressionContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeSimpleExpr(vhdlParser::Simple_expressionContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeAggregate(vhdlParser::AggregateContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeRelation(vhdlParser::RelationContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeTerm(vhdlParser::TermContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeFactor(vhdlParser::FactorContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makePrimary(vhdlParser::PrimaryContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeShiftExpr(vhdlParser::Shift_expressionContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeChoices(vhdlParser::ChoicesContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeChoice(vhdlParser::ChoiceContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeName(vhdlParser::NameContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeCallExpr(ast::Expr base, vhdlParser::Function_call_or_indexed_name_partContext *ctx)
      -> ast::Expr;
    [[nodiscard]]
    auto makeSliceExpr(ast::Expr base, vhdlParser::Slice_name_partContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeSelectExpr(ast::Expr base, vhdlParser::Selected_name_partContext *ctx) -> ast::Expr;
    [[nodiscard]]
    auto makeAttributeExpr(ast::Expr base, vhdlParser::Attribute_name_partContext *ctx)
      -> ast::Expr;
    [[nodiscard]]
    auto makeCallArgument(vhdlParser::Association_elementContext *ctx) -> ast::Expr;

    // Constraints
    [[nodiscard]]
    auto makeIndexConstraint(vhdlParser::Index_constraintContext *ctx)
      -> std::vector<ast::BinaryExpr>;
    [[nodiscard]]
    auto makeRangeConstraint(vhdlParser::Range_constraintContext *ctx)
      -> std::vector<ast::BinaryExpr>;

  private:
    // Helper methods to reduce boilerplate for common AST node creation
    template<typename Ctx>
    [[nodiscard]]
    auto makeBinary(Ctx *ctx, std::string op, ast::Expr left, ast::Expr right) -> ast::Expr
    {
        return expr_helpers::makeBinary(
          trivia_, ctx, std::move(op), std::move(left), std::move(right));
    }

    template<typename Ctx>
    [[nodiscard]]
    auto makeUnary(Ctx *ctx, std::string op, ast::Expr value) -> ast::Expr
    {
        return expr_helpers::makeUnary(trivia_, ctx, std::move(op), std::move(value));
    }

    template<typename Ctx>
    [[nodiscard]]
    auto makeToken(Ctx *ctx, std::string text) -> ast::Expr
    {
        return expr_helpers::makeToken(trivia_, ctx, std::move(text));
    }
};

} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
