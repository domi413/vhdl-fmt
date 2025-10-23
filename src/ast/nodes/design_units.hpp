#ifndef AST_NODES_ENTITY_HPP
#define AST_NODES_ENTITY_HPP

#include "ast/nodes/declarations.hpp"
#include "ast/nodes/statements.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace ast {

struct DesignUnit : Node
{
  protected:
    DesignUnit() = default;
};

struct GenericClause final : Visitable<GenericClause>
{
    std::vector<std::unique_ptr<GenericParam>> generics;
};

struct PortClause final : Visitable<PortClause>
{
    std::vector<std::unique_ptr<Port>> ports;
};

struct Entity final : Visitable<Entity, DesignUnit>
{
    std::string name;
    std::unique_ptr<GenericClause> generic_clause;
    std::unique_ptr<PortClause> port_clause;
    std::vector<std::unique_ptr<Declaration>> decls;
    std::vector<std::unique_ptr<Statement>> stmts;
    std::optional<std::string> end_label;
};

struct Architecture final : Visitable<Architecture, DesignUnit>
{
    std::string name;
    std::string entity_name;
    std::vector<std::unique_ptr<Declaration>> decls;
    std::vector<std::unique_ptr<Statement>> stmts;
};

} // namespace ast

#endif /* AST_NODES_ENTITY_HPP */
