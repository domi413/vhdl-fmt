#ifndef AST_NODES_EXPRESSIONS_HPP
#define AST_NODES_EXPRESSIONS_HPP

#include "ast/node.hpp"

#include <memory>
#include <string>
#include <vector>

namespace ast {

/// Base class for all expression nodes.
struct Expr : Node
{};

/// Single token: literal, identifier, or operator.
struct TokenExpr : Visitable<TokenExpr, Expr>
{
    std::string text; ///< Literal text of the token.
};

/// Aggregate or grouped list of expressions (e.g. `(others => '0')`).
struct GroupExpr : Visitable<GroupExpr, Expr>
{
    std::vector<std::unique_ptr<Expr>> children; ///< Ordered child expressions.
};

/// Unary expression (e.g. `-a`, `not ready`).
struct UnaryExpr : Visitable<UnaryExpr, Expr>
{
    std::string op;              ///< Unary operator symbol.
    std::unique_ptr<Expr> value; ///< Operand expression.
};

/// Binary expression (e.g. `a + b`, `x downto 0`).
struct BinaryExpr : Visitable<BinaryExpr, Expr>
{
    std::unique_ptr<Expr> left;  ///< Left operand.
    std::string op;              ///< Binary operator symbol.
    std::unique_ptr<Expr> right; ///< Right operand.
};

/// Explicit parentheses around an expression (e.g. `(a + b)`).
struct ParenExpr : Visitable<ParenExpr, Expr>
{
    std::unique_ptr<Expr> inner; ///< Inner expression inside parentheses.
};

} // namespace ast

#endif /* AST_NODES_EXPRESSIONS_HPP */
