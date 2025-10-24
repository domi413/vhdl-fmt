#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "tree/ParseTree.h"
#include "vhdlParser.h"

#include <CommonTokenStream.h>
#include <concepts>
#include <functional>

namespace builder {

template<typename T>
concept Traversal = requires(T t, antlr4::tree::ParseTree *node) {
    { t.dispatch(node) } -> std::same_as<void>;
    { t.walk(node) } -> std::same_as<void>;
};

class Translator
{
    std::vector<ast::DesignUnit> *units_{ nullptr }; ///< Destination for top-level design units
    std::vector<ast::Declaration> *decls_{ nullptr }; ///< Destination for declarations (signals, constants, etc.)
    std::vector<ast::Statement> *stmts_{ nullptr }; ///< Destination for statements
    TriviaBinder &trivia_;
    antlr4::CommonTokenStream &tokens_;
    std::function<void(antlr4::tree::ParseTree *)> dispatch_;
    std::function<void(antlr4::tree::ParseTree *)> walk_;

    /// @brief Helper to create a boxed expression
    template<typename T = ast::Expr>
    [[nodiscard]]
    static auto box(T &&expr) -> ast::Box<T>
    {
        return std::make_unique<T>(std::forward<T>(expr));
    }

  public:
    Translator(TriviaBinder &tv, antlr4::CommonTokenStream &tokens) :
      trivia_(tv),
      tokens_(tokens)
    {
    }

    /// @brief Set the destination vector for design units
    void setUnitsDestination(std::vector<ast::DesignUnit> &units) { units_ = &units; }

    /// @brief Set the destination vector for declarations (used when building entity/architecture bodies)
    void setDeclsDestination(std::vector<ast::Declaration> &decls) { decls_ = &decls; }
    
    /// @brief Clear the declarations destination
    void clearDeclsDestination() { decls_ = nullptr; }

    /// @brief Set the destination vector for statements (used when building architecture/process bodies)
    void setStmtsDestination(std::vector<ast::Statement> &stmts) { stmts_ = &stmts; }
    
    /// @brief Clear the statements destination
    void clearStmtsDestination() { stmts_ = nullptr; }

    /// @brief Push a declaration to the current destination (if set)
    void pushDecl(ast::Declaration &&decl)
    {
        if (decls_ != nullptr) {
            decls_->push_back(std::move(decl));
        }
    }

    /// @brief Push a statement to the current destination (if set)
    void pushStmt(ast::Statement &&stmt)
    {
        if (stmts_ != nullptr) {
            stmts_->push_back(std::move(stmt));
        }
    }

    ~Translator() = default;

    Translator(const Translator &) = delete;
    auto operator=(const Translator &) -> Translator & = delete;
    Translator(Translator &&) = delete;
    auto operator=(Translator &&) -> Translator & = delete;

    template<Traversal T>
    void attachTraversal(T &traversal)
    {
        dispatch_ = [&traversal](antlr4::tree::ParseTree *node) { traversal.dispatch(node); };
        walk_ = [&traversal](antlr4::tree::ParseTree *node) { traversal.walk(node); };
    }

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
};

} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
