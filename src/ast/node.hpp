#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <cstddef>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ast {

struct Comments
{
    std::string text;
};

/// @brief Represents intentional vertical spacing (1+ blank lines) between code elements.
/// Only captured when there are 2+ newlines (which creates 1+ visible blank lines).
/// Used to preserve user's intentional grouping while allowing the formatter to normalize spacing.
struct ParagraphBreak
{
    std::size_t blank_lines{ 1 }; ///< Number of visible blank lines
};

/// @brief A variant representing either a comment or a paragraph break to preserve order.
using Trivia = std::variant<Comments, ParagraphBreak>;

/// @brief Container for leading and trailing trivia (Newlines are only counted leading).
struct NodeTrivia
{
    std::vector<Trivia> leading;
    std::vector<Comments> trailing;
};

/// @brief Abstract base class for all AST nodes.
/// @note Do not instantiate directly - There is no virtual destructor to leverage aggregate
/// initialization.
struct NodeBase
{
    std::optional<NodeTrivia> trivia;
};

} // namespace ast

#endif /* AST_NODE_HPP */
