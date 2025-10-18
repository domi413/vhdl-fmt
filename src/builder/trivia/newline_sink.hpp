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
    /// @brief Push newline trivia into the node’s trivia list if any breaks exist.
    constexpr void push(ast::Node::NodeTrivia &dst, std::size_t breaks)
    {
        if (breaks == 0) {
            return;
        }

        ast::Newlines n{ breaks };
        dst.leading.emplace_back(n);
    }
};

} // namespace builder

// NOLINTEND(readability-convert-member-functions-to-static)
#endif /* BUILDER_TRIVIA_NEWLINE_SINK_HPP */
