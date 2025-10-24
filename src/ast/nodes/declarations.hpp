#ifndef AST_NODES_DECLARATIONS_HPP
#define AST_NODES_DECLARATIONS_HPP

#include "ast/node.hpp"
#include "nodes/expressions.hpp"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ast {

// Forward declarations
struct ConstantDecl;
struct SignalDecl;
struct GenericParam;
struct Port;

/// Variant type for all declarations
using Declaration = std::variant<
    ConstantDecl,
    SignalDecl,
    GenericParam,
    Port
>;

// Base data common to all declarations
struct DeclBase : NodeBase
{
    std::vector<std::string> names;
};

// Constant declaration: constant WIDTH : integer := 8;
struct ConstantDecl : DeclBase
{
    std::string type_name;
    std::optional<Expr> init_expr;
};

// Signal declaration: signal v : std_logic_vector(7 downto 0) := (others => '0');
struct SignalDecl : DeclBase
{
    std::string type_name;
    bool has_bus_kw{ false };
    std::vector<BinaryExpr> constraints;
    std::optional<Expr> init_expr;
};

// Generic parameter inside GENERIC clause
struct GenericParam : DeclBase
{
    std::string type_name;
    std::optional<Expr> default_expr;
};

// Port entry inside PORT clause
struct Port : DeclBase
{
    std::string mode; // "in" / "out"
    std::string type_name;
    std::optional<Expr> default_expr;
    std::vector<BinaryExpr> constraints;
};

} // namespace ast

#endif /* AST_NODES_DECLARATIONS_HPP */