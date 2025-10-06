#ifndef AST_DESIGN_FILE_HPP
#define AST_DESIGN_FILE_HPP

#include "ast/node.hpp"

#include <memory>

namespace ast {

struct DesignFile : Node
{
    std::vector<std::unique_ptr<Node>> units;
    void accept(ASTVisitor &v) const override { v.visit(*this); }
};

} // namespace ast

#endif /* AST_DESIGN_FILE_HPP */
