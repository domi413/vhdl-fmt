#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include "ast/visitor.hpp"

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace ast {

/// @brief Represents comments or newlines attached to an AST node.
struct Trivia
{
    enum class Kind
    {
        Comment,
        Newlines
    };
    Kind kind{ Kind::Comment };

    // For Kind::Comment
    std::string text;

    // For Kind::Newlines: number of line breaks (>=1).
    std::size_t breaks{ 0 };
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
        std::vector<Trivia> leading;  ///< Trivia appearing before the node.
        std::vector<Trivia> trailing; ///< Trivia appearing after the node.
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
