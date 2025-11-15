#ifndef BUILDER_TRIVIA_TRIVIA_BINDER_HPP
#define BUILDER_TRIVIA_TRIVIA_BINDER_HPP

#include "CommonTokenStream.h"
#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "builder/trivia/comment_sink.hpp"
#include "builder/trivia/newline_sink.hpp"
#include "vhdlLexer.h"

#include <cstddef>

namespace builder {

/// @brief Builds ordered trivia streams (comments + newlines) for AST nodes.
class TriviaBinder final
{
  public:
    explicit TriviaBinder(antlr4::CommonTokenStream &ts) noexcept : tokens_(ts) {}

    ~TriviaBinder() = default;

    TriviaBinder(const TriviaBinder &) = delete;
    auto operator=(const TriviaBinder &) -> TriviaBinder & = delete;
    TriviaBinder(TriviaBinder &&) = delete;
    auto operator=(TriviaBinder &&) -> TriviaBinder & = delete;

    /// @brief Binds collected trivia to the specified AST node.
    void bind(ast::NodeBase &node, const antlr4::ParserRuleContext *ctx);

  private:
    struct AnchorToken
    {
        std::size_t index{};
        std::size_t line{};
    };

    antlr4::CommonTokenStream &tokens_;
    CommentSink comments_;
    NewlineSink newlines_;

    [[nodiscard]]
    static constexpr auto isComment(const antlr4::Token *t) noexcept -> bool
    {
        return (t != nullptr) && t->getChannel() == vhdlLexer::COMMENTS;
    }

    [[nodiscard]]
    static constexpr auto isNewline(const antlr4::Token *t) noexcept -> bool
    {
        return (t != nullptr) && t->getChannel() == vhdlLexer::NEWLINES;
    }

    [[nodiscard]]
    static constexpr auto isDefault(const antlr4::Token *t) noexcept -> bool
    {
        return (t != nullptr) && t->getChannel() == vhdlLexer::DEFAULT_TOKEN_CHANNEL;
    }

    void collectLeading(ast::NodeTrivia &dst, std::size_t start_index);
    void collectTrailing(ast::NodeTrivia &dst, std::size_t stop_index);

    auto collectInline(ast::NodeTrivia &dst, std::size_t stop_index) -> antlr4::Token *;

    auto findLastDefaultOnLine(std::size_t start_index) const -> std::size_t;
};

} // namespace builder

#endif /* BUILDER_TRIVIA_TRIVIA_BINDER_HPP */
