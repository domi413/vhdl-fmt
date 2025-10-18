#ifndef AST_NODES_DECLARATIONS_HPP
#define AST_NODES_DECLARATIONS_HPP

#include "ast/node.hpp"

#include <memory>
#include <string>

namespace ast {

struct GenericParam : Visitable<GenericParam>
{
    std::string type;
    std::vector<std::string> names;
    std::optional<std::string> init;
};

struct Range : Visitable<Range>
{
    std::string left_expr;  // e.g. "DATA_WIDTH - 1"
    std::string direction;  // "downto" or "to"
    std::string right_expr; // e.g. "0"
};

struct Port : Visitable<Port>
{
    std::string mode, type;
    std::vector<std::string> names;
    std::vector<std::unique_ptr<ast::Range>> constraints;
    std::optional<std::string> init;
};

struct Entity : Visitable<Entity>
{
    std::string name;
    std::vector<std::unique_ptr<Port>> ports;
    std::vector<std::unique_ptr<GenericParam>> generics;
};

} // namespace ast

#endif /* AST_NODES_DECLARATIONS_HPP */
