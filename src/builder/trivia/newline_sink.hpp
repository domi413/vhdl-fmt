#ifndef BUILDER_TRIVIA_NEWLINE_SINK_HPP
#define BUILDER_TRIVIA_NEWLINE_SINK_HPP
// NOLINTBEGIN(readability-convert-member-functions-to-static)

#include "ast/node.hpp"

#include <vector>

namespace builder {

/// @brief Appends paragraph break trivia to a node.
/// Only called when there are 2+ newlines (creating 1+ visible blank lines).
class NewlineSink final
{
  public:
    /// @brief Push paragraph break trivia. Expects newline_count >= 2.
    constexpr void push(std::vector<ast::Trivia> &dst, unsigned int newline_count)
    {
        if (newline_count < 2) {
            return;
        }

        dst.emplace_back(ast::ParagraphBreak{ newline_count - 1 });
    }
};

} // namespace builder

// NOLINTEND(readability-convert-member-functions-to-static)
#endif /* BUILDER_TRIVIA_NEWLINE_SINK_HPP */
