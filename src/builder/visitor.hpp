#ifndef BUILDER_VISITOR_HPP
#define BUILDER_VISITOR_HPP

#include "builder/adapter/base_void_visitor.hpp"
#include "translator.hpp"
#include "vhdlParser.h"

namespace builder {

/// @brief CST visitor that builds AST nodes via `Translator`.
///
/// Walks the ANTLR4 VHDL parse tree and delegates node construction
/// to the associated `Translator`. Contains traversal logic only.
class Visitor : public adapter::BaseVoidVisitor
{
    Translator &translator_; ///< Translator used for node creation.

  public:
    /// @brief Create a visitor bound to a translator.
    explicit Visitor(Translator &t) : translator_(t) { translator_.attachTraversal(*this); }

    ~Visitor() override = default;
    Visitor(const Visitor &) = delete;
    auto operator=(const Visitor &) -> Visitor & = delete;
    Visitor(Visitor &&) = delete;
    auto operator=(Visitor &&) -> Visitor & = delete;

    // Entity / Architecture
    void visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx) override;
    void visitArchitectureBody(vhdlParser::Architecture_bodyContext *ctx) override;

    // Clauses
    void visitGenericClause(vhdlParser::Generic_clauseContext *ctx) override;
    void visitPortClause(vhdlParser::Port_clauseContext *ctx) override;

    // Declarations
    void visitConstantDeclaration(vhdlParser::Constant_declarationContext *ctx) override;
    void visitSignalDeclaration(vhdlParser::Signal_declarationContext *ctx) override;

    // Constraints
    void visitExplicitRange(vhdlParser::Explicit_rangeContext *ctx) override;

    // Expressions
    void visitExpression(vhdlParser::ExpressionContext *ctx) override;
    void visitSimpleExpression(vhdlParser::Simple_expressionContext *ctx) override;
    void visitAggregate(vhdlParser::AggregateContext *ctx) override;
    void visitRelation(vhdlParser::RelationContext *ctx) override;
    void visitTerm(vhdlParser::TermContext *ctx) override;
    void visitFactor(vhdlParser::FactorContext *ctx) override;
    void visitPrimary(vhdlParser::PrimaryContext *ctx) override;
    void visitShiftExpression(vhdlParser::Shift_expressionContext *ctx) override;
    void visitChoices(vhdlParser::ChoicesContext *ctx) override;
    void visitChoice(vhdlParser::ChoiceContext *ctx) override;
};

} // namespace builder

#endif /* BUILDER_VISITOR_HPP */
