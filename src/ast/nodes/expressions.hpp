#ifndef AST_NODES_EXPRESSIONS_HPP
#define AST_NODES_EXPRESSIONS_HPP

#include "ast/node.hpp"

#include <memory>
#include <string>
#include <vector>

namespace ast {

// Forward declarations
struct TokenExpr;
struct GroupExpr;
struct UnaryExpr;
struct BinaryExpr;
struct ParenExpr;

/// Variant type for all expressions
using Expr = std::variant<
    TokenExpr,
    GroupExpr,
    UnaryExpr,
    BinaryExpr,
    ParenExpr
>;

/// Single token: literal, identifier, or operator.
struct TokenExpr : NodeBase
{
    std::string text; ///< Literal text of the token.
};

/// Aggregate or grouped list of expressions (e.g. `(others => '0')`).
struct GroupExpr : NodeBase
{
    std::vector<std::unique_ptr<Expr>> children; ///< Ordered child expressions.
};

/// Unary expression (e.g. `-a`, `not ready`).
struct UnaryExpr : NodeBase
{
    std::string op;              ///< Unary operator symbol.
    std::unique_ptr<Expr> value; ///< Operand expression.
};

/// Binary expression (e.g. `a + b`, `x downto 0`).
struct BinaryExpr : NodeBase
{
    std::unique_ptr<Expr> left;  ///< Left operand.
    std::string op;              ///< Binary operator symbol.
    std::unique_ptr<Expr> right; ///< Right operand.
};

/// Explicit parentheses around an expression (e.g. `(a + b)`).
struct ParenExpr : NodeBase
{
    std::unique_ptr<Expr> inner; ///< Inner expression inside parentheses.
};

} // namespace ast

#endif /* AST_NODES_EXPRESSIONS_HPP */
