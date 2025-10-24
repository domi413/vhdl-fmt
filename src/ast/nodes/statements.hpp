#ifndef AST_NODES_STATEMENTS_HPP
#define AST_NODES_STATEMENTS_HPP

#include "ast/node.hpp"
#include "ast/nodes/expressions.hpp"

#include <string>
#include <variant>
#include <vector>

namespace ast {

// Forward declarations
struct ConcurrentAssign;
struct Process;

/// Variant type for all statements
using Statement = std::variant<ConcurrentAssign, Process>;

// Concurrent signal assignment
struct ConcurrentAssign : NodeBase
{
    Expr target;
    Expr value;
};

// Simplified process statement
struct Process : NodeBase
{
    std::vector<std::string> sensitivity_list;
    std::vector<Statement> body;
};

} // namespace ast

#endif /* AST_NODES_STATEMENTS_HPP */