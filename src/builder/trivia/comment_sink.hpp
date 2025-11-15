#ifndef BUILDER_TRIVIA_COMMENT_SINK_HPP
#define BUILDER_TRIVIA_COMMENT_SINK_HPP

#include "Token.h"
#include "ast/node.hpp"

#include <cstddef>
#include <optional>
#include <unordered_set>
#include <utility>
#include <vector>

namespace builder {

/// @brief Appends comment trivia to a node, with global de-duplication by token index.
class CommentSink final
{
  public:
    CommentSink() = default;

    /// @brief Push a comment token into the node’s comment list, unless already added.
    void push(std::vector<ast::Trivia> &dst, const antlr4::Token *t)
    {
        if (t == nullptr) {
            return;
        }

        const auto idx = t->getTokenIndex();
        if (!used_.insert(idx).second) {
            return; // already added elsewhere
        }

        const ast::Comment c{ t->getText() };

        dst.emplace_back(c);
    }

    /// @brief Push a comment token into the node’s comment list, unless already added.
    void push(std::optional<ast::Comment> &dst, const antlr4::Token *t)
    {
        if (t == nullptr) {
            return;
        }

        const auto idx = t->getTokenIndex();
        if (!used_.insert(idx).second) {
            return; // already added elsewhere
        }

        const ast::Comment c{ t->getText() };

        dst.emplace(c);
    }

    auto wasTokenUsed(std::size_t idx) const noexcept -> bool { return used_.contains(idx); }

  private:
    std::unordered_set<std::size_t> used_; ///< set of token indices already added
};

} // namespace builder

#endif /* BUILDER_TRIVIA_COMMENT_SINK_HPP */
