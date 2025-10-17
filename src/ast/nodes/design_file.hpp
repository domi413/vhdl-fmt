#ifndef AST_NODES_DESIGN_FILE_HPP
#define AST_NODES_DESIGN_FILE_HPP

#include "ast/node.hpp"

#include <memory>
#include <vector>

namespace ast {

struct DesignUnit : Visitable<DesignUnit>
{
    virtual ~DesignUnit() = default;
};

struct DesignFile : Visitable<DesignFile>
{
    std::vector<std::unique_ptr<DesignUnit>> units;
};

} // namespace ast

#endif /* AST_NODES_DESIGN_FILE_HPP */
