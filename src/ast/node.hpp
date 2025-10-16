#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include "ast/visitor.hpp"

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

struct Newlines
{
    std::size_t breaks{ 1 };
};

/// @brief A variant representing either a comment or a newline to preserve order.
using Trivia = std::variant<Comments, Newlines>;

/// @brief Base class for all AST nodes.
struct Node
{
    Node() = default;
    virtual ~Node() = default;

    Node(const Node &) = default;
    auto operator=(const Node &) -> Node & = default;
    Node(Node &&) = default;
    auto operator=(Node &&) -> Node & = default;

    /// @brief Container for leading and trailing trivia (Newlines are only counted leading).
    struct NodeTrivia
    {
        std::vector<Trivia> leading;
        std::vector<Comments> trailing;
    };

    /// @brief Accept a visitor for dynamic dispatch.
    virtual void accept(Visitor &v) const = 0;

    /// @brief Create and return this node’s comment block.
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
