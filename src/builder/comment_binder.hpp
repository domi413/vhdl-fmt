#ifndef BUILDER_COMMENT_BINDER_HPP
#define BUILDER_COMMENT_BINDER_HPP

#include "CommonTokenStream.h"
#include "ParserRuleContext.h"
#include "Token.h"
#include "ast/node.hpp"
#include "vhdlLexer.h"

#include <cstddef>
#include <string>
#include <unordered_set>
#include <vector>

namespace builder {

/// @brief Attaches source comments to AST nodes from hidden channels.
///
/// Collects from `COMMENTS` and `NEWLINES`, attaching:
/// - **Leading:** nearest contiguous `COMMENTS` block above the start token,
///   bounded by a blank line (≥2 line breaks) or a non-hidden token.
/// - **Inline:** `COMMENTS` on the same line as the stop token before the first `NEWLINES`.
/// Each comment token is attached at most once globally.
class CommentBinder
{
  public:
    /// @brief Construct with the given token stream (must expose hidden channels).
    explicit CommentBinder(antlr4::CommonTokenStream &ts) : tokens(ts) {}

    ~CommentBinder() = default;
    CommentBinder(const CommentBinder &) = delete;
    auto operator=(const CommentBinder &) -> CommentBinder & = delete;
    CommentBinder(CommentBinder &&) = delete;
    auto operator=(CommentBinder &&) -> CommentBinder & = delete;

    /// @brief Attach leading and inline comments around the context’s token range.
    void bind(ast::Node &node, const antlr4::ParserRuleContext *ctx);

  private:
    struct StopInfo
    {
        std::size_t idx;  ///< Stop token index in the stream.
        std::size_t line; ///< Stop token source line.
    };

    antlr4::CommonTokenStream &tokens;    ///< Token stream (owner elsewhere).
    std::unordered_set<std::size_t> used; ///< Deduplication set of attached comment token indices.

    // channel helpers
    static auto isComment(const antlr4::Token *t) -> bool
    {
        return (t != nullptr) && t->getChannel() == vhdlLexer::COMMENTS;
    }
    static auto isNewline(const antlr4::Token *t) -> bool
    {
        return (t != nullptr) && t->getChannel() == vhdlLexer::NEWLINES;
    }
    /// @brief Count '\n' in a NEWLINES token; returns ≥1 for robustness.
    static auto countLineBreaks(const std::string &s) -> std::size_t
    {
        std::size_t n = 0;
        for (const char c : s) {
            if (c == '\n') {
                ++n;
            }
        }
        return n > 0 ? n : 1;
    }

    // collectors
    void collectLeading(ast::Node::NodeComments &dst, std::size_t start_idx);
    void collectInline(ast::Node::NodeComments &dst, StopInfo stop);

    // insertion with deduplication
    void pushIfFresh(const antlr4::Token *t, std::vector<ast::Comment> &vec, bool inline_flag);
};

} // namespace builder

#endif /* BUILDER_COMMENT_BINDER_HPP */
