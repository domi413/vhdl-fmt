#ifndef BUILDER_TRIVIA_NEWLINE_SINK_HPP
#define BUILDER_TRIVIA_NEWLINE_SINK_HPP
// NOLINTBEGIN(readability-convert-member-functions-to-static)

#include "ast/node.hpp"

#include <cstddef>

namespace builder {

/// @brief Appends newline trivia (aggregated line breaks) to a node.
class NewlineSink
{
  public:
    /// @brief Push newline trivia into the node’s comment list if any breaks exist.
    static constexpr void push(ast::Node::NodeComments &dst, bool to_leading, std::size_t breaks)
    {
        if (breaks == 0) {
            return;
        }

        ast::NewlinesTrivia n{ breaks };
        auto &vec = to_leading ? dst.leading : dst.trailing;
        vec.emplace_back(n);
    }
};

} // namespace builder

#endif /* BUILDER_TRIVIA_NEWLINE_SINK_HPP */
