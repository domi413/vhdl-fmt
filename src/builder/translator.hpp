#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "vhdlParser.h"

#include <CommonTokenStream.h>

namespace builder {

class Translator final
{
    TriviaBinder &trivia_;
    antlr4::CommonTokenStream &tokens_;

    /// @brief Helper to create a boxed expression
    template<typename T = ast::Expr>
    [[nodiscard]]
    static auto box(T &&expr) -> ast::Box<T>
    {
        return std::make_unique<T>(std::forward<T>(expr));
    }

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
};

} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
