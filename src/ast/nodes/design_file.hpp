#ifndef AST_NODES_DESIGN_FILE_HPP
#define AST_NODES_DESIGN_FILE_HPP

#include "ast/node.hpp"

#include <memory>

namespace ast {

struct DesignFile : Visitable<DesignFile>
{
    std::vector<std::unique_ptr<Node>> units;
};

} // namespace ast

#endif /* AST_NODES_DESIGN_FILE_HPP */
