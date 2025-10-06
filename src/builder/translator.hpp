#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "CommonTokenStream.h"
#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "builder/assembly/assembler.hpp"
#include "vhdlParser.h"

#include <unordered_set>

namespace builder {

/**
 * @brief Builds semantic AST nodes from CST contexts.
 *
 * The `Translator` converts ANTLR CST nodes into AST representations.
 *
 * Responsibilities:
 *  - Construct AST nodes from parser contexts
 *  - Attach comments and source metadata
 *  - Delegate structural operations to `Assembler`
 *
 * Contains no traversal logic — called by `Visitor` during tree walking.
 */
class Translator
{
  public:
    Translator(Assembler &b, antlr4::CommonTokenStream &ts) : builder(b), tokens(ts) {}

    ~Translator() = default;
    Translator(const Translator &) = delete;
    auto operator=(const Translator &) -> Translator & = delete;
    Translator(Translator &&) = delete;
    auto operator=(Translator &&) -> Translator & = delete;

    // ---- Scoped assembly helpers ----
    template<typename Vec, typename Fn>
    void into(Vec &vec, Fn &&fn)
    {
        builder.into(vec, std::forward<Fn>(fn));
    }

    template<typename Vec>
    auto with(Vec &vec)
    {
        return builder.with(vec);
    }

  private:
    Assembler &builder;
    antlr4::CommonTokenStream &tokens;
    std::unordered_set<std::size_t> consumed_comment_token_indices;

    // ---- Node creation and metadata helpers ----
    template<typename T>
    auto spawn(antlr4::ParserRuleContext *ctx) -> T &
    {
        auto &node = builder.spawn<T>();
        attachComments(node, ctx);
        return node;
    }

    void attachComments(ast::Node &node, const antlr4::ParserRuleContext *ctx);

  public:
    // ---- AST node constructors ----
    auto makeEntity(vhdlParser::Entity_declarationContext *ctx) -> ast::Entity &;
    auto makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
      -> ast::GenericParam &;
    auto makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx) -> ast::Port &;
    auto makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Range &;
};

} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
