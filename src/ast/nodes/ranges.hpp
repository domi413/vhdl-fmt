#ifndef AST_NODES_RANGES_HPP
#define AST_NODES_RANGES_HPP

#include "ast/node.hpp"
#include "ast/nodes/expressions.hpp"

#include <memory>
#include <string>

namespace ast {

/// @brief Represents a discrete range (e.g. "7 downto 0", "0 to 15").
///
/// A Range is not an expression — it’s a type constraint.
/// However, its left and right bounds are expressions.
struct Range : Visitable<Range>
{
    std::unique_ptr<Expr> left;
    std::string direction; // "to" or "downto"
    std::unique_ptr<Expr> right;
};

} // namespace ast

#endif /* AST_NODES_RANGES_HPP */
