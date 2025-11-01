#ifndef AST_NODES_DESIGN_FILE_HPP
#define AST_NODES_DESIGN_FILE_HPP

#include "ast/node.hpp"
#include "ast/nodes/design_units.hpp"

#include <vector>

namespace ast {

struct DesignFile : NodeBase
{
    std::vector<DesignUnit> units;
};

} // namespace ast

#endif /* AST_NODES_DESIGN_FILE_HPP */
