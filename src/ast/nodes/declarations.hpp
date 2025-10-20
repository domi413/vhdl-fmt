#ifndef AST_NODES_DECLARATIONS_HPP
#define AST_NODES_DECLARATIONS_HPP

#include "ast/node.hpp"
#include "nodes/expressions.hpp"

#include <memory>
#include <string>
#include <vector>

namespace ast {

// Abstract base for all declarations
struct Declaration : Node
{
    std::vector<std::string> names;
};

// Constant declaration: constant WIDTH : integer := 8;
struct ConstantDecl : Visitable<ConstantDecl, Declaration>
{
    std::string type_name;
    std::unique_ptr<Expr> init_expr;
};

// Signal declaration: signal v : std_logic_vector(7 downto 0) := (others => '0');
struct SignalDecl : Visitable<SignalDecl, Declaration>
{
    std::string type_name;
    bool has_bus_kw{ false };
    std::vector<std::unique_ptr<BinaryExpr>> constraints;
    std::unique_ptr<Expr> init_expr;
};

// Generic parameter inside GENERIC clause
struct GenericParam : Visitable<GenericParam, Declaration>
{
    std::string type_name;
    std::unique_ptr<Expr> default_expr;
};

// Port entry inside PORT clause
struct Port : Visitable<Port, Declaration>
{
    std::string mode; // "in" / "out"
    std::string type_name;
    std::unique_ptr<Expr> default_expr;
    std::vector<std::unique_ptr<BinaryExpr>> constraints;
};

} // namespace ast

#endif /* AST_NODES_DECLARATIONS_HPP */
