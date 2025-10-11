#ifndef AST_VISITOR_BASE_HPP
#define AST_VISITOR_BASE_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/visitor.hpp"

#include <concepts>
#include <memory>

namespace ast {

template<typename T>
concept AstNode = std::derived_from<T, ast::Node>;

/**
 * @brief Default recursive AST visitor.
 *
 * Provides base implementations that traverse all child nodes.
 *
 * **Responsibilities:**
 *  - Recursively walk the AST hierarchy.
 *  - Serve as a base for specialized visitors.
 */
class BaseVisitor : public Visitor
{
  protected:
    /**
     * @brief Visit a node if it exists.
     *
     * Safely invokes `accept()` on a node pointer, allowing
     * traversal of optional or nullable children.
     */
    template<AstNode T>
    void dispatch(const std::unique_ptr<T> &node)
    {
        if (node != nullptr) {
            node->accept(*this);
        }
    }

    /**
     * @brief Continue recursive traversal.
     *
     * Calls the default `visit()` implementation for the given node,
     * allowing derived visitors to resume walking after custom logic.
     */
    template<AstNode T>
    void walk(const T &node)
    {
        BaseVisitor::visit(node);
    }

  public:
    // -- Node visitors --
    void visit(const DesignFile &node) override
    {
        for (const std::unique_ptr<Node> &u : node.units) {
            dispatch(u);
        }
    }

    void visit(const Entity &node) override
    {
        for (const auto &g : node.generics) {
            dispatch(g);
        }
        for (const auto &p : node.ports) {
            dispatch(p);
        }
    }

    void visit(const Port &node) override
    {
        for (const auto &r : node.constraints) {
            dispatch(r);
        }
    }

    // -- Leaf visitors --
    void visit(const GenericParam &node) override { (void)node; }
    void visit(const Range &node) override { (void)node; }
};

} // namespace ast

#endif /* AST_VISITOR_BASE_HPP */
