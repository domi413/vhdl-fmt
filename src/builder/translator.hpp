#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "CommonTokenStream.h"
#include "ParserRuleContext.h"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/entity.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/assembly/assembler.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "vhdlParser.h"

namespace builder {

class Translator
{
  public:
    Translator(Assembler &ass, antlr4::CommonTokenStream &tokens) :
      assembler_(ass),
      tokens_(tokens),
      trivia_(tokens)
    {
    }

    ~Translator() = default;
    Translator(const Translator &) = delete;
    auto operator=(const Translator &) -> Translator & = delete;
    Translator(Translator &&) = delete;
    auto operator=(Translator &&) -> Translator & = delete;

    template<typename Dest, typename Fn>
    void into(Dest &dest, Fn &&fn)
    {
        assembler_.into(dest, std::forward<Fn>(fn));
    }

  private:
    Assembler &assembler_;
    antlr4::CommonTokenStream &tokens_;
    TriviaBinder trivia_;

    template<typename T>
    auto spawn(antlr4::ParserRuleContext *ctx) -> T &
    {
        auto &node = assembler_.spawn<T>();
        trivia_.bind(node, ctx);
        return node;
    }

  public:
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
    auto makeExpr(vhdlParser::Simple_expressionContext *ctx) -> ast::Expr &;
};

} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
