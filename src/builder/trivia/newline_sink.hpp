#ifndef BUILDER_TRIVIA_NEWLINE_SINK_HPP
#define BUILDER_TRIVIA_NEWLINE_SINK_HPP
// NOLINTBEGIN(readability-convert-member-functions-to-static)

#include "ast/node.hpp"

#include <cstddef>

namespace builder {

/// @brief Appends paragraph break trivia to a node.
/// Only called when there are 2+ newlines (creating 1+ visible blank lines).
class NewlineSink final
{
  public:
    /// @brief Push paragraph break trivia. Expects newline_count >= 2.
    constexpr void push(ast::NodeTrivia &dst, std::size_t newline_count)
    {
        if (newline_count < 2) {
            return;
        }

        // Convert newline count to blank line count
        const std::size_t blank_lines = newline_count - 1;
        ast::ParagraphBreak pb{ blank_lines };
        dst.leading.emplace_back(pb);
    }
};

} // namespace builder

// NOLINTEND(readability-convert-member-functions-to-static)
#endif /* BUILDER_TRIVIA_NEWLINE_SINK_HPP */
