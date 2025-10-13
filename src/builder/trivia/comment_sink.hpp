#ifndef BUILDER_TRIVIA_COMMENT_SINK_HPP
#define BUILDER_TRIVIA_COMMENT_SINK_HPP

#include "Token.h"
#include "ast/node.hpp"

#include <cstddef>
#include <unordered_set>

namespace builder {

/// @brief Appends comment trivia to a node, with global de-duplication by token index.
class CommentSink
{
  public:
    CommentSink() = default;

    /// @brief Push a comment token into the node’s comment list, unless already added.
    void push(ast::Node::NodeComments &dst, bool to_leading, const antlr4::Token *t)
    {
        if (t == nullptr) {
            return;
        }

        const auto idx = t->getTokenIndex();
        if (!used_.insert(idx).second) {
            return; // duplicate token already handled
        }

        ast::CommentTrivia c{ t->getText() };
        auto &vec = to_leading ? dst.leading : dst.trailing;
        vec.emplace_back(std::move(c)); // stores as variant<CommentTrivia, NewlinesTrivia>
    }

  private:
    std::unordered_set<std::size_t> used_; ///< set of token indices already added
};

} // namespace builder

#endif // BUILDER_TRIVIA_COMMENT_SINK_HPP
