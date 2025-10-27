#ifndef AST_NODES_EXPRESSIONS_HPP
#define AST_NODES_EXPRESSIONS_HPP

#include "ast/node.hpp"

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace ast {

// Forward declarations
struct TokenExpr;
struct GroupExpr;
struct UnaryExpr;
struct BinaryExpr;
struct ParenExpr;
struct CallExpr;

/// Helper alias for boxed recursive types
template<typename T>
using Box = std::unique_ptr<T>;

/// Variant type for all expressions (holds values, not pointers)
using Expr = std::variant<TokenExpr, GroupExpr, UnaryExpr, BinaryExpr, ParenExpr, CallExpr>;

/// Single token: literal, identifier, or operator.
struct TokenExpr : NodeBase
{
    std::string text; ///< Literal text of the token.
};

/// Aggregate or grouped list of expressions (e.g. `(others => '0')`).
struct GroupExpr : NodeBase
{
    std::vector<Expr> children; ///< Ordered child expressions.
};

/// Unary expression (e.g. `-a`, `not ready`).
struct UnaryExpr : NodeBase
{
    std::string op;  ///< Unary operator symbol.
    Box<Expr> value; ///< Operand expression (boxed for recursion).
};

/// Binary expression (e.g. `a + b`, `x downto 0`).
struct BinaryExpr : NodeBase
{
    Box<Expr> left;  ///< Left operand (boxed for recursion).
    std::string op;  ///< Binary operator symbol.
    Box<Expr> right; ///< Right operand (boxed for recursion).
};

/// Explicit parentheses around an expression (e.g. `(a + b)`).
struct ParenExpr : NodeBase
{
    Box<Expr> inner; ///< Inner expression inside parentheses (boxed for recursion).
};

/// Function call or indexed name (e.g. `rising_edge(clk)`, `data(7 downto 0)`).
/// Note: VHDL doesn't syntactically distinguish function calls from array indexing.
struct CallExpr : NodeBase
{
    Box<Expr> callee; ///< Function/array name being called/indexed.
    Box<Expr> args;   ///< Arguments (single expr or GroupExpr for multiple args).
};

} // namespace ast

#endif /* AST_NODES_EXPRESSIONS_HPP */
