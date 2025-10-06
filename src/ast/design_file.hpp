#pragma once
#include "ast/node.hpp"

#include <memory>

namespace ast {

struct DesignFile : Node
{
    std::vector<std::unique_ptr<Node>>
      units; // NOLINT(misc-non-private-member-variables-in-classes)
    void accept(ASTVisitor &v) const override { v.visit(*this); }
};

} // namespace ast
