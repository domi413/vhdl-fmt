#ifndef AST_VISITOR_BASE_HPP
#define AST_VISITOR_BASE_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/visitor.hpp"

#include <concepts>
#include <memory>
#include <vector>

namespace ast {

template<typename T>
concept AstNode = std::derived_from<T, ast::Node>;

/// @brief Default recursive AST visitor.
///
/// Provides traversal logic for visiting all child nodes.
/// Serves as a reusable base for visitors performing formatting,
/// analysis, or transformation.
class BaseVisitor : public Visitor
{
  protected:
    /// @brief Safely visit a node if it exists.
    ///
    /// Invokes `accept()` on the given pointer, allowing traversal of
    /// optional or nullable children.
    template<AstNode T>
    void dispatch(const std::unique_ptr<T> &node)
    {
        if (node != nullptr) {
            node->accept(*this);
        }
    }

    template<AstNode T>
    void dispatchAll(const std::vector<std::unique_ptr<T>> &vec)
    {
        for (const auto &node : vec) {
            if (node != nullptr) {
                node->accept(*this);
            }
        }
    }

    /// @brief Continue recursive traversal from a specific node.
    ///
    /// Calls the default `visit()` implementation, enabling derived
    /// visitors to resume automatic child traversal after custom logic.
    template<AstNode T>
    void walk(const T &node)
    {
        BaseVisitor::visit(node);
    }

  public:
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
