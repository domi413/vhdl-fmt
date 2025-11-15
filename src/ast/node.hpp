#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ast {

struct Comment
{
    std::string text;
};

/// @brief Represents intentional vertical spacing (1+ blank lines) between code elements.
/// Only captured when there are 2+ newlines (which creates 1+ visible blank lines).
/// Used to preserve user's intentional grouping while allowing the formatter to normalize spacing.
struct ParagraphBreak
{
    unsigned int blank_lines{ 1 }; ///< Number of visible blank lines
};

/// @brief A variant representing either a comment or a paragraph break to preserve order.
using Trivia = std::variant<Comment, ParagraphBreak>;

/// @brief Container for leading and trailing trivia (Newlines are only counted leading).
struct NodeTrivia
{
    std::vector<Trivia> leading;
    std::vector<Trivia> trailing;
    std::optional<Comment> inline_comment;
};

/// @brief Abstract base class for all AST nodes - Do not instantiate directly.
/// @note There is no virtual destructor to leverage aggregate initialization.
struct NodeBase
{
    std::optional<NodeTrivia> trivia;
};

} // namespace ast

#endif /* AST_NODE_HPP */
