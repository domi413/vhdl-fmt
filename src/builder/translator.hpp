#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "CommonTokenStream.h"
#include "ParserRuleContext.h"
#include "ast/nodes/declarations.hpp"
#include "builder/assembly/assembler.hpp"
#include "builder/trivia/trivia_binder.hpp"
#include "vhdlParser.h"

namespace builder {

/// @brief Converts CST nodes into semantic AST structures.
///
/// Builds `ast::Node` objects from ANTLR parse contexts,
/// attaches metadata, and delegates structural insertion to `Assembler`.
class Translator
{
  public:
    /// @brief Construct a translator bound to an assembler and token stream.
    Translator(Assembler &b, antlr4::CommonTokenStream &ts) : builder(b), tokens(ts), trivia(ts) {}
    ~Translator() = default;

    Translator(const Translator &) = delete;
    auto operator=(const Translator &) -> Translator & = delete;
    Translator(Translator &&) = delete;
    auto operator=(Translator &&) -> Translator & = delete;

    /// @brief Execute a block with temporary output redirection.
    template<typename Vec, typename Fn>
    void into(Vec &vec, Fn &&fn)
    {
        builder.into(vec, std::forward<Fn>(fn));
    }

  private:
    Assembler &builder;                ///< Active assembler used for node creation.
    antlr4::CommonTokenStream &tokens; ///< Token stream for comment attachment.
    TriviaBinder trivia;               ///< Trivia binder for attaching trivia to nodes.

    /// @brief Create and register a node, attaching relevant comments.
    template<typename T>
    auto spawn(antlr4::ParserRuleContext *ctx) -> T &
    {
        auto &node{ builder.spawn<T>() };
        trivia.bind(node, ctx);
        return node;
    }

  public:
    auto makeEntity(vhdlParser::Entity_declarationContext *ctx) -> ast::Entity &;
    auto makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
      -> ast::GenericParam &;
    auto makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx) -> ast::Port &;
    auto makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Range &;
};

} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
