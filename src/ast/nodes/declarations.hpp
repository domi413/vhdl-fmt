#ifndef AST_NODES_DECLARATIONS_HPP
#define AST_NODES_DECLARATIONS_HPP

#include "ast/node.hpp"
#include "ast/visitor.hpp"

#include <memory>
#include <string>

namespace ast {

struct GenericParam : Node
{
    std::string type;
    std::vector<std::string> names;
    std::optional<std::string> init;

    void accept(Visitor &v) const override { v.visit(*this); }
};

struct Range : Node
{
    std::string left_expr;  // e.g. "DATA_WIDTH - 1"
    std::string direction;  // "downto" or "to"
    std::string right_expr; // e.g. "0"

    void accept(Visitor &v) const override { v.visit(*this); }
};

struct Port : Node
{
    std::string mode, type;
    std::vector<std::string> names;
    std::vector<std::unique_ptr<ast::Range>> constraints;
    std::optional<std::string> init;

    void accept(Visitor &v) const override { v.visit(*this); }
};

struct Entity : Node
{
    std::string name;
    std::vector<std::unique_ptr<Port>> ports;
    std::vector<std::unique_ptr<GenericParam>> generics;
    void accept(Visitor &v) const override { v.visit(*this); }
};

} // namespace ast

#endif /* AST_NODES_DECLARATIONS_HPP */
