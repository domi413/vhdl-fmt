#ifndef AST_VISITOR_BASE_HPP
#define AST_VISITOR_BASE_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/visitor.hpp"

#include <concepts>

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
class BaseVisitor : Visitor
{
  protected:
    /**
     * @brief Visit a node if it exists.
     *
     * Safely invokes `accept()` on a node pointer, allowing
     * traversal of optional or nullable children.
     */
    void dispatch(Node *node)
    {
        if (node != nullptr) {
            node->accept(*this);
        }
    }

  public:
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

    // Node visitors
    void visit(const DesignFile &node) override
    {
        for (const auto &u : node.units) {
            dispatch(u.get());
        }
    }

    void visit(const Entity &node) override
    {
        // Recurse into generics and ports
        for (const auto &g : node.generics) {
            dispatch(g.get());
        }
        for (const auto &p : node.ports) {
            dispatch(p.get());
        }
    }

    void visit(const GenericParam &node) override { (void)node; }

    void visit(const Port &node) override
    {
        for (const auto &r : node.constraints) {
            dispatch(r.get());
        }
    }

    void visit(const Range &node) override { (void)node; }
};

} // namespace ast

#endif /* AST_VISITOR_BASE_HPP */
