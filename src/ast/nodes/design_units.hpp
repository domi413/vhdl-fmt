#ifndef AST_NODES_ENTITY_HPP
#define AST_NODES_ENTITY_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/statements.hpp"

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ast {

// Forward declarations
struct Entity;
struct Architecture;

/// Variant type for all design units
using DesignUnit = std::variant<
    Entity,
    Architecture
>;

struct GenericClause : NodeBase
{
    std::vector<std::unique_ptr<GenericParam>> generics;
};

struct PortClause : NodeBase
{
    std::vector<std::unique_ptr<Port>> ports;
};

struct Entity : NodeBase
{
    std::string name;
    std::unique_ptr<GenericClause> generic_clause;
    std::unique_ptr<PortClause> port_clause;
    std::vector<std::unique_ptr<Declaration>> decls;
    std::vector<std::unique_ptr<Statement>> stmts;
    std::optional<std::string> end_label;
};

struct Architecture : NodeBase
{
    std::string name;
    std::string entity_name;
    std::vector<std::unique_ptr<Declaration>> decls;
    std::vector<std::unique_ptr<Statement>> stmts;
};

} // namespace ast

#endif /* AST_NODES_ENTITY_HPP */