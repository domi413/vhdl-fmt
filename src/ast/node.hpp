#pragma once
#include "ast/visitor.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace ast {

struct Comment
{
    enum class Kind : std::uint8_t
    {
        line,
        block
    };

    std::string text;
    Kind kind;
    int line = 0;
    bool is_inline = false;
};

struct Node
{
    Node() = default;
    virtual ~Node() = default;
    Node(const Node &) = default;
    auto operator=(const Node &) -> Node & = default;
    Node(Node &&) = default;
    auto operator=(Node &&) -> Node & = default;

    struct NodeComments
    {
        std::vector<Comment> leading;
        std::vector<Comment> trailing;
    };

    virtual void accept(ASTVisitor &v) const = 0;
    auto getComments() -> NodeComments & { return comments.emplace(); }
    [[nodiscard]] auto tryGetComments() const -> const std::optional<NodeComments> &
    {
        return comments;
    }

  private:
    std::optional<NodeComments> comments;
};

} // namespace ast
