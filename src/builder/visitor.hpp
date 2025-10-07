#ifndef BUILDER_VISITOR_HPP
#define BUILDER_VISITOR_HPP

#include "builder/adapter/base_void_visitor.hpp"
#include "translator.hpp"
#include "tree/ParseTree.h"
#include "vhdlParser.h"

namespace builder {

/**
 * @brief Traverses ANTLR contexts (CST nodes).
 *
 * The `Visitor` walks the parse tree produced by the ANTLR4 VHDL grammar
 * and delegates semantic node creation to `Translator`.
 *
 * Responsibilities:
 *  - Traverse ANTLR contexts (CST nodes)
 *  - Route subtrees to the correct AST construction functions
 *  - Maintain traversal structure (e.g., visitEntity, visitPort)
 *  - Pure traversal logic; no AST construction occurs here directly
 */
class Visitor : public adapter::BaseVoidVisitor
{
    Translator &translator;

  public:
    explicit Visitor(Translator &t) : translator(t) {};

    ~Visitor() override = default;
    Visitor(const Visitor &) = delete;
    auto operator=(const Visitor &) -> Visitor & = delete;
    Visitor(Visitor &&) = delete;
    auto operator=(Visitor &&) -> Visitor & = delete;

    void walk(antlr4::tree::ParseTree *ctx) { adapter::BaseVoidVisitor::walk(ctx); }

    void visitEntityDeclaration(vhdlParser::Entity_declarationContext *ctx) override;
    void visitGenericClause(vhdlParser::Generic_clauseContext *ctx) override;
    void visitPortClause(vhdlParser::Port_clauseContext *ctx) override;
    void visitConstraint(vhdlParser::ConstraintContext *ctx) override;
};

} // namespace builder

#endif /* BUILDER_VISITOR_HPP */
