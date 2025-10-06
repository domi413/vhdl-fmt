#ifndef BUILDER_TRANSLATOR_HPP
#define BUILDER_TRANSLATOR_HPP

#include "CommonTokenStream.h"
#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "builder/assembly/node_builder.hpp"
#include "vhdlLexer.h"
#include "vhdlParser.h"

#include <cstddef>
#include <unordered_set>
#include <vector>

namespace builder {

/**
 * @brief Builds semantic AST nodes from CST contexts.
 *
 * The `Translator` converts ANTLR CST nodes into AST representations.
 *
 * Responsibilities:
 *  - Construct AST nodes from parser contexts
 *  - Attach comments and source metadata
 *  - Delegate structural operations to `NodeBuilder`
 *
 * Contains no traversal logic — called by `Visitor` during tree walking.
 */
class Translator
{
    NodeAssembler &builder;
    antlr4::CommonTokenStream &tokens;

  public:
    Translator(NodeAssembler &b, antlr4::CommonTokenStream &ts) : builder(b), tokens(ts) {}

    ~Translator() = default;
    Translator(const Translator &) = delete;
    auto operator=(const Translator &) -> Translator & = delete;
    Translator(Translator &&) = delete;
    auto operator=(Translator &&) -> Translator & = delete;

    // ---- AST node constructors ----
    auto makeEntity(vhdlParser::Entity_declarationContext *ctx) -> ast::Entity &;
    auto makeGenericParam(vhdlParser::Interface_constant_declarationContext *ctx)
      -> ast::GenericParam &;
    auto makeSignalPort(vhdlParser::Interface_port_declarationContext *ctx) -> ast::Port &;
    auto makeRange(vhdlParser::Explicit_rangeContext *ctx) -> ast::Range &;

    // ---- Builder forwarding helpers ----
    template<typename T>
    auto spawn() -> T &
    {
        return builder.spawn<T>();
    }

    template<typename T>
    auto spawn(antlr4::ParserRuleContext *ctx) -> T &
    {
        auto &node = builder.spawn<T>();
        attachComments(node, ctx);
        return node;
    }

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
    std::unordered_set<std::size_t> consumed_comment_token_indices;

    void attachComments(ast::Node &node, const antlr4::ParserRuleContext *ctx)
    {
        if (ctx == nullptr) {
            return;
        }
        auto &cm = node.getComments();
        auto kind = [](const antlr4::Token *t) {
            return t->getType() == vhdlLexer::COMMENT ? ast::Comment::Kind::line
                                                      : ast::Comment::Kind::block;
        };
        auto push = [&](const antlr4::Token *t, std::vector<ast::Comment> &dst, bool is_inline) {
            if (!t) {
                return;
            }
            const std::size_t idx = t->getTokenIndex();
            if (consumed_comment_token_indices.contains(idx)) {
                return;
            }
            consumed_comment_token_indices.insert(idx);
            dst.push_back({ t->getText(), kind(t), static_cast<int>(t->getLine()), is_inline });
        };

        // Leading: everything hidden to the left of start (consume all)
        for (const auto *t : tokens.getHiddenTokensToLeft(ctx->getStart()->getTokenIndex())) {
            push(t, cm.leading, false);
        }

        // Trailing: only inline. Standalone comments are left
        // unconsumed here so the next node will pick them up as leading.
        const std::size_t stop_line = ctx->getStop()->getLine();
        for (const auto *t : tokens.getHiddenTokensToRight(ctx->getStop()->getTokenIndex())) {
            if (t == nullptr) {
                continue;
            }
            if (t->getLine() == stop_line) {
                push(t, cm.trailing, true);
            }
        }
    }
};
} // namespace builder

#endif /* BUILDER_TRANSLATOR_HPP */
