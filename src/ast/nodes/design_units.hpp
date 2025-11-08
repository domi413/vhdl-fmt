#ifndef AST_NODES_ENTITY_HPP
#define AST_NODES_ENTITY_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/statements.hpp"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ast {

// Forward declarations
struct EntityDecl;
struct ArchitectureBody;

/// Variant type for all design units (holds values, not pointers)
using DesignUnit = std::variant<EntityDecl, ArchitectureBody>;

struct GenericClause : NodeBase
{
    std::vector<InterfaceConstantDecl> generics;
};

struct PortClause : NodeBase
{
    std::vector<InterfacePortDecl> ports;
};

struct EntityDecl : NodeBase
{
    std::string name;
    GenericClause generic_clause;
    PortClause port_clause;
    std::vector<Declaration> decls;
    std::vector<ConcurrentStatement> stmts;
    std::optional<std::string> end_label;
};

struct ArchitectureBody : NodeBase
{
    std::string name;
    std::string entity_name;
    std::vector<Declaration> decls;
    std::vector<ConcurrentStatement> stmts;
};

} // namespace ast

#endif /* AST_NODES_ENTITY_HPP */
