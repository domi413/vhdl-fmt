#ifndef BUILDER_TRIVIA_TRIVIA_BINDER_HPP
#define BUILDER_TRIVIA_TRIVIA_BINDER_HPP

#include "CommonTokenStream.h"
#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "builder/trivia/comment_sink.hpp"
#include "builder/trivia/newline_sink.hpp"
#include "vhdlLexer.h"

#include <algorithm>
#include <cstddef>
#include <string_view>

namespace builder {

/// @brief Builds ordered trivia streams (comments + newlines) for AST nodes.
///
/// The TriviaBinder inspects lexer tokens to collect leading and trailing
/// trivia (comments and newline runs) and attaches them to AST nodes.
class TriviaBinder
{
  public:
    explicit TriviaBinder(antlr4::CommonTokenStream &ts) noexcept : tokens_(ts) {}

    ~TriviaBinder() = default;

    TriviaBinder(const TriviaBinder &) = delete;
    auto operator=(const TriviaBinder &) -> TriviaBinder & = delete;
    TriviaBinder(TriviaBinder &&) = delete;
    auto operator=(TriviaBinder &&) -> TriviaBinder & = delete;

    /// @brief Binds collected trivia to the specified AST node.
    void bind(ast::Node &node, const antlr4::ParserRuleContext *ctx);

  private:
    struct StopInfo
    {
        std::size_t idx{};
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
    static constexpr auto countLineBreaks(std::string_view s) noexcept -> std::size_t
    {
        const auto n = static_cast<std::size_t>(std::ranges::count(s, '\n'));
        return std::max<std::size_t>(1, n);
    }

    void collectLeading(ast::Node::NodeComments &dst, std::size_t start_idx);
    void collectTrailing(ast::Node::NodeComments &dst, const StopInfo &stop);
};

} // namespace builder

#endif /* BUILDER_TRIVIA_TRIVIA_BINDER_HPP */
