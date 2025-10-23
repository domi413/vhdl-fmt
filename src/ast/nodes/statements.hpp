#ifndef AST_NODES_STATEMENTS_HPP
#define AST_NODES_STATEMENTS_HPP

#include "ast/node.hpp"
#include "ast/nodes/expressions.hpp"

#include <memory>
#include <vector>

namespace ast {

struct Statement : Node
{
  protected:
    Statement() = default;
};

// Concurrent signal assignment
struct ConcurrentAssign : Visitable<ConcurrentAssign, Statement>
{
    std::unique_ptr<Expr> target;
    std::unique_ptr<Expr> value;
};

// Simplified process statement
struct Process : Visitable<Process, Statement>
{
    std::vector<std::string> sensitivity_list;
    std::vector<std::unique_ptr<Statement>> body;
};

} // namespace ast

#endif /* AST_NODES_STATEMENTS_HPP */
