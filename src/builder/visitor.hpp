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
    explicit Visitor(Translator &t) : translator_(t) {}

    ~Visitor() override = default;
    Visitor(const Visitor &) = delete;
    auto operator=(const Visitor &) -> Visitor & = delete;
    Visitor(Visitor &&) = delete;
    auto operator=(Visitor &&) -> Visitor & = delete;

    // Node visitors
    void visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx) override;
    void visitGenericClause(vhdlParser::Generic_clauseContext *ctx) override;
    void visitPortClause(vhdlParser::Port_clauseContext *ctx) override;
    void visitConstraint(vhdlParser::ConstraintContext *ctx) override;
};

} // namespace builder

#endif /* BUILDER_VISITOR_HPP */
