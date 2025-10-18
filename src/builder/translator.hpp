#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "ParserRuleContext.h"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/entity.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/ranges.hpp"
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
    template<typename T>
    auto spawn(antlr4::ParserRuleContext *ctx) -> T &
    {
        auto &node = assembler_.spawn<T>();
        trivia_.bind(node, ctx);
        return node;
    }

    /// @brief Spawn a new AST node of type T without binding trivia.
    template<typename T>
    auto spawn() -> T &
    {
        auto &node = assembler_.spawn<T>();
        return node;
    }

    /// @brief Helper to populate a destination container via a lambda.
    template<typename Dest, typename Fn>
    void into(Dest &dest, Fn &&fn)
    {
        assembler_.into(dest, std::forward<Fn>(fn));
    }

    /**
     *  TODO(dyb): This is currently a hack to tokenize expressions.
     *  This should later be removed for proper expression handling.
     */
    auto makeTokenizedExpr(antlr4::ParserRuleContext *ctx) -> ast::Expr &;

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
    auto makeEntity(vhdlParser::Entity_declarationContext *ctx) -> ast::Entity &;
    auto makeArchitecture(vhdlParser::Architecture_bodyContext *ctx) -> ast::Architecture &;

    // Clauses
    auto makeGenericClause(vhdlParser::Generic_clauseContext *ctx) -> ast::GenericClause &;
    auto makePortClause(vhdlParser::Port_clauseContext *ctx) -> ast::PortClause &;

    // Declarations
    auto makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
      -> ast::GenericParam &;
    auto makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx) -> ast::Port &;
    auto makeConstantDecl(vhdlParser::Constant_declarationContext *ctx) -> ast::ConstantDecl &;
    auto makeSignalDecl(vhdlParser::Signal_declarationContext *ctx) -> ast::SignalDecl &;

    // Constraints
    auto makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Range &;

    // Expressions
    auto makeExpr(vhdlParser::ExpressionContext *ctx) -> ast::Expr &;
    auto makeSimpleExpr(vhdlParser::Simple_expressionContext *ctx) -> ast::Expr &;
};

} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
