#ifndef BUILDER_TRIVIA_COMMENT_SINK_HPP
#define BUILDER_TRIVIA_COMMENT_SINK_HPP

#include "Token.h"
#include "ast/node.hpp"

#include <cstddef>
#include <unordered_set>
#include <utility>

namespace builder {

/// @brief Appends comment trivia to a node, with global de-duplication by token index.
class CommentSink final
{
  public:
    CommentSink() = default;

    /// @brief Push a comment token into the node’s comment list, unless already added.
    void push(ast::NodeTrivia &dst, const bool to_leading, const antlr4::Token *t)
    {
        if (t == nullptr) {
            return;
        }

        if (const auto idx = static_cast<std::size_t>(t->getTokenIndex());
            !used_.insert(idx).second) {
            return; // already added elsewhere
        }

        if (const ast::Comments c{ t->getText() }; to_leading) {
            dst.leading.emplace_back(c);
        } else {
            dst.trailing.emplace_back(c);
        }
    }

  private:
    std::unordered_set<std::size_t> used_; ///< set of token indices already added
};

} // namespace builder

#endif /* BUILDER_TRIVIA_COMMENT_SINK_HPP */
