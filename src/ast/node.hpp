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
struct NodeBase
{
    NodeBase() = default;
    virtual ~NodeBase() = default;

    NodeBase(const NodeBase &) = default;
    auto operator=(const NodeBase &) -> NodeBase & = default;
    NodeBase(NodeBase &&) = default;
    auto operator=(NodeBase &&) -> NodeBase & = default;

    /// @brief Create and return this node's comment block.
    auto emplaceTrivia() -> NodeTrivia & { return trivia_.emplace(); }

    /// @brief Return attached comments if present.
    [[nodiscard]]
    auto tryGetTrivia() const -> const std::optional<NodeTrivia> &
    {
        return trivia_;
    }

    /// @brief Check if this node has any trivia attached.
    [[nodiscard]]
    auto hasTrivia() const -> bool
    {
        return trivia_.has_value();
    }

    /// @brief Return leading trivia (comments + newlines) if present, else empty.
    [[nodiscard]]
    auto leading() const -> const std::vector<Trivia> &
    {
        static const auto e = std::vector<Trivia>{};
        return trivia_ ? trivia_->leading : e;
    }

    /// @brief Return trailing comments if present, else empty.
    [[nodiscard]]
    auto trailing() const -> const std::vector<Comments> &
    {
        static const auto e = std::vector<Comments>{};
        return trivia_ ? trivia_->trailing : e;
    }

  private:
    std::optional<NodeTrivia> trivia_;
};

} // namespace ast

#endif /* AST_NODE_HPP */
