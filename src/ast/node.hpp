#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include "ast/visitor.hpp"

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace ast {

/// @brief Represents a source comment attached to an AST node.
struct Comment
{
    std::string text;        ///< Raw comment text.
    std::size_t line{ 0 };   ///< Line number in source.
};

/// @brief Base class for all AST nodes.
///
/// Defines the common interface for traversal, comment attachment,
/// and polymorphic destruction.
struct Node
{
    Node() = default;
    virtual ~Node() = default;

    Node(const Node &) = default;
    auto operator=(const Node &) -> Node & = default;
    Node(Node &&) = default;
    auto operator=(Node &&) -> Node & = default;

    /// @brief Container for leading and trailing comments.
    struct NodeComments
    {
        std::vector<Comment> leading;  ///< Comments appearing before the node.
        std::vector<Comment> trailing; ///< Comments appearing after the node.
    };

    /// @brief Accept a visitor for dynamic dispatch.
    virtual void accept(Visitor &v) const = 0;

    /// @brief Create and return this node’s comment block.
    auto getComments() -> NodeComments & { return comments.emplace(); }

    /// @brief Return attached comments if present.
    [[nodiscard]] auto tryGetComments() const -> const std::optional<NodeComments> &
    {
        return this->comments;
    }

  private:
    std::optional<NodeComments> comments; ///< Optional comment data.
};

} // namespace ast

#endif /* AST_NODE_HPP */
