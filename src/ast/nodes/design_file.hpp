#ifndef AST_NODES_DESIGN_FILE_HPP
#define AST_NODES_DESIGN_FILE_HPP

#include "ast/node.hpp"
#include "ast/nodes/design_units.hpp"

#include <memory>
#include <vector>

namespace ast {

struct DesignFile final : Visitable<DesignFile>
{
    std::vector<std::unique_ptr<DesignUnit>> units;
};

} // namespace ast

#endif /* AST_NODES_DESIGN_FILE_HPP */
