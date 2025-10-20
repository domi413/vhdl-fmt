#ifndef AST_NODES_EXPRESSIONS_HPP
#define AST_NODES_EXPRESSIONS_HPP

#include "ast/node.hpp"

#include <memory>
#include <string>

namespace ast {

struct Expr : Node
{};

struct TokenExpr : Visitable<TokenExpr, Expr>
{
    std::string text; // literal token, operator, identifier
};

struct GroupExpr : Visitable<GroupExpr, Expr>
{
    std::vector<std::unique_ptr<Expr>> children; // ordered tokens and subgroups
};

struct UnaryExpr : Visitable<UnaryExpr, Expr>
{
    std::string op;
    std::unique_ptr<Expr> value;
};

struct BinaryExpr : Visitable<BinaryExpr, Expr>
{
    std::unique_ptr<Expr> left;
    std::string op;
    std::unique_ptr<Expr> right;
};

struct ParenExpr : Visitable<ParenExpr, Expr>
{
    std::unique_ptr<Expr> inner;
};

} // namespace ast

#endif /* AST_NODES_EXPRESSIONS_HPP */
