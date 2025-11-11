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
    void push(ast::NodeTrivia &dst, bool to_leading, const antlr4::Token *t)
    {
        if (t == nullptr) {
            return;
        }

        const auto idx = t->getTokenIndex();
        if (!used_.insert(idx).second) {
            return; // already added elsewhere
        }

        const ast::Comments c{ t->getText() };

        if (to_leading) {
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
