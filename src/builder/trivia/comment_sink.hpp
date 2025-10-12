#ifndef BUILDER_TRIVIA_COMMENT_SINK_HPP
#define BUILDER_TRIVIA_COMMENT_SINK_HPP

#include "Token.h"
#include "ast/node.hpp"

#include <cstddef>
#include <unordered_set>
#include <utility>

namespace builder {

/// @brief Appends comment trivia to a node, with global de-duplication by token index.
class CommentSink
{
  public:
    CommentSink() = default;

    void push(ast::Node::NodeComments &dst, bool to_leading, const antlr4::Token *t)
    {
        if (!t)
            return;
        const std::size_t idx = static_cast<std::size_t>(t->getTokenIndex());
        if (!used_.insert(idx).second)
            return;

        ast::Trivia tv;
        tv.kind = ast::Trivia::Kind::Comment;
        tv.text = t->getText();

        auto &vec = to_leading ? dst.leading : dst.trailing;
        vec.push_back(std::move(tv));
    }

  private:
    std::unordered_set<std::size_t> used_;
};

} // namespace builder

#endif /* BUILDER_TRIVIA_COMMENT_SINK_HPP */
