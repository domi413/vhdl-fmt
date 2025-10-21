#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "ParserRuleContext.h"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/entity.hpp"
#include "builder/assembly/assembler.hpp"
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
    Assembler &assembler_;
    TriviaBinder &trivia_;
    antlr4::CommonTokenStream &tokens_;
    std::function<void(antlr4::tree::ParseTree *)> dispatch_;
    std::function<void(antlr4::tree::ParseTree *)> walk_;

    /// @brief Spawn a new AST node of type T, binding trivia from the given context.
    template<typename T, typename F>
    void spawn(antlr4::ParserRuleContext *ctx, bool trivia, F &&fn)
    {
        auto &node = assembler_.spawn<T>();
        if (trivia) {
            trivia_.bind(node, ctx);
        }
        std::forward<F>(fn)(node);
    }

    /// @brief Helper to populate a destination container via a lambda.
    template<typename Dest, typename Fn>
    void into(Dest &dest, Fn &&fn)
    {
        assembler_.into(dest, std::forward<Fn>(fn));
    }

  public:
    Translator(Assembler &ass, TriviaBinder &tv, antlr4::CommonTokenStream &tokens) :
      assembler_(ass),
      trivia_(tv),
      tokens_(tokens)
    {
    }

    ~Translator() = default;

    Translator(const Translator &) = delete;
    auto operator=(const Translator &) -> Translator & = delete;
    Translator(Translator &&) = delete;
    auto operator=(Translator &&) -> Translator & = delete;

    template<Traversal T>
    void attachTraversal(T &trav)
    {
        dispatch_ = [&trav](antlr4::tree::ParseTree *node) { trav.dispatch(node); };
        walk_ = [&trav](antlr4::tree::ParseTree *node) { trav.walk(node); };
    }

    // Design units
    void makeEntity(vhdlParser::Entity_declarationContext *ctx);
    void makeArchitecture(vhdlParser::Architecture_bodyContext *ctx);

    // Clauses
    void makeGenericClause(vhdlParser::Generic_clauseContext *ctx);
    void makePortClause(vhdlParser::Port_clauseContext *ctx);

    // Declarations
    void makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx);
    void makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx);
    void makeConstantDecl(vhdlParser::Constant_declarationContext *ctx);
    void makeSignalDecl(vhdlParser::Signal_declarationContext *ctx);

    // Expressions
    void makeExpr(vhdlParser::ExpressionContext *ctx);
    void makeSimpleExpr(vhdlParser::Simple_expressionContext *ctx);
    void makeAggregate(vhdlParser::AggregateContext *ctx);
    void makeRelation(vhdlParser::RelationContext *ctx);
    void makeTerm(vhdlParser::TermContext *ctx);
    void makeFactor(vhdlParser::FactorContext *ctx);
    void makePrimary(vhdlParser::PrimaryContext *ctx);
    void makeShiftExpr(vhdlParser::Shift_expressionContext *ctx);
    void makeChoices(vhdlParser::ChoicesContext *ctx);
    void makeChoice(vhdlParser::ChoiceContext *ctx);
    void makeRange(vhdlParser::Explicit_rangeContext *ctx);
};

} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
