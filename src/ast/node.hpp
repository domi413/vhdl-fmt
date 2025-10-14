#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include "ast/visitor.hpp"

#include <cstddef>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ast {

struct CommentTrivia
{
    std::string text;
};

struct NewlinesTrivia
{
    std::size_t breaks{ 1 };
};

/// @brief A variant representing either a comment or a newline to preserve order.
using Trivia = std::variant<CommentTrivia, NewlinesTrivia>;

/// @brief Base class for all AST nodes.
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
        std::vector<Trivia> leading;
        std::vector<Trivia> trailing;
    };

    /// @brief Accept a visitor for dynamic dispatch.
    virtual void accept(Visitor &v) const = 0;

    /// @brief Create and return this node’s comment block.
    auto emplaceComments() -> NodeComments & { return comments_.emplace(); }

    /// @brief Return attached comments if present.
    [[nodiscard]]
    auto tryGetComments() const -> const std::optional<NodeComments> &
    {
        return comments_;
    }

  private:
    std::optional<NodeComments> comments_;
};

} // namespace ast

#endif /* AST_NODE_HPP */
