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
struct InterfaceConstantDecl;
struct InterfacePortDecl;
struct AliasDecl;

/// Variant type for all declarations
using Declaration
  = std::variant<ConstantDecl, SignalDecl, InterfaceConstantDecl, InterfacePortDecl, AliasDecl>;

// Base data common to all declarations
struct DeclBase : NodeBase
{
    std::vector<std::string> names;
};

// Constant declaration: constant WIDTH : integer := 8;
struct ConstantDecl : DeclBase
{
    std::string type_name;
    std::optional<Expression> init_expr;
};

// Signal declaration: signal v : std_logic_vector(7 downto 0) := (others => '0');
struct SignalDecl : DeclBase
{
    std::string type_name;
    bool has_bus_kw{ false };
    std::vector<BinaryExpr> constraints;
    std::optional<Expression> init_expr;
};

// Generic parameter inside GENERIC clause
struct InterfaceConstantDecl : DeclBase
{
    std::string type_name;
    std::optional<Expression> default_expr;
};

// Port entry inside PORT clause
struct InterfacePortDecl : DeclBase
{
    std::string mode; // "in" / "out"
    std::string type_name;
    std::optional<Expression> default_expr;
    std::vector<BinaryExpr> constraints;
};

/// Alias declaration: alias ShortName : type_mark is target_name;
/// Can alias objects (signals, variables, constants) or non-objects (types, subprograms)
struct AliasDecl : NodeBase
{
    std::string name; ///< Alias designator (identifier or character literal)
    std::optional<std::string> type_indication; ///< Optional subtype indication for object aliases
    Expression target;                    ///< Name being aliased (can include slices/selections)
    std::optional<std::string> signature; ///< Optional signature for subprogram/enum disambiguation
};

} // namespace ast

#endif /* AST_NODES_DECLARATIONS_HPP */
