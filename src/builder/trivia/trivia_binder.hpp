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
///
/// Implements a *left-stick* policy:
/// every comment or newline sequence appearing before a node in source order
/// is attached to that node as **leading** trivia, regardless of spacing or
/// paragraph boundaries. This ensures that all comment blocks and blank-line
/// runs are preserved and follow the next syntactic construct.
///
/// Trailing trivia only captures **inline** comments that occur on the same
/// line as a node’s final token; vertical spacing below a node is always
/// considered part of the next node’s leading trivia.
///
/// The resulting trivia stream maintains the original ordering of comments
/// and newlines exactly as they appear in the source, enabling precise
/// round-tripping and faithful pretty-printing.
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
        return (t != nullptr) && (t->getChannel() == vhdlLexer::COMMENTS);
    }

    [[nodiscard]]
    static constexpr auto isNewline(const antlr4::Token *t) noexcept -> bool
    {
        return (t != nullptr) && (t->getChannel() == vhdlLexer::NEWLINES);
    }

    [[nodiscard]]
    static constexpr auto isDefault(const antlr4::Token *t) noexcept -> bool
    {
        return (t != nullptr) && (t->getChannel() == vhdlLexer::DEFAULT_TOKEN_CHANNEL);
    }

    [[nodiscard]]
    auto findLastDefaultOnLine(std::size_t start_index) const noexcept -> std::size_t;

    void collectLeading(ast::NodeTrivia &dst, std::size_t start_index);
    void collectTrailing(ast::NodeTrivia &dst, const AnchorToken &anchor);
};

} // namespace builder

#endif /* BUILDER_TRIVIA_TRIVIA_BINDER_HPP */
