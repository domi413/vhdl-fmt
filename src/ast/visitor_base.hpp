#ifndef AST_VISITOR_BASE_HPP
#define AST_VISITOR_BASE_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/entity.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
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
/// Derived visitors can override specific `visit()` methods to customize behavior,
/// and call `walk(node)` to resume default traversal.
class BaseVisitor : public Visitor
{
  protected:
    /// @brief Safely visit a node if it exists.
    template<AstNode T>
    void dispatch(const std::unique_ptr<T> &node)
    {
        if (node) {
            node->accept(*this);
        }
    }

    /// @brief Visit all elements of a vector of nodes.
    template<AstNode T>
    void dispatchAll(const std::vector<std::unique_ptr<T>> &vec)
    {
        for (const auto &node : vec) {
            dispatch(node);
        }
    }

    /// @brief Continue default traversal manually.
    template<AstNode T>
    void walk(const T &node)
    {
        this->BaseVisitor::visit(node);
    }

  public:
    // ---------------------------------------------------------------------
    // Core hierarchy
    // ---------------------------------------------------------------------
    void visit(const Node &node) override
    {
        (void)node; // abstract root, nothing to traverse
    }

    void visit(const DesignFile &node) override { dispatchAll(node.units); }

    void visit(const DesignUnit &node) override
    {
        (void)node; // abstract
    }

    // ---------------------------------------------------------------------
    // Declarations
    // ---------------------------------------------------------------------
    void visit(const Declaration &node) override
    {
        (void)node; // abstract
    }

    void visit(const ConstantDecl &node) override { walk(static_cast<const Declaration &>(node)); }

    void visit(const SignalDecl &node) override
    {
        dispatchAll(node.constraints);
        dispatch(node.init_expr);
        walk(static_cast<const Declaration &>(node));
    }

    void visit(const GenericParam &node) override
    {
        dispatch(node.default_expr);
        walk(static_cast<const Declaration &>(node));
    }

    void visit(const Port &node) override
    {
        dispatchAll(node.constraints);
        dispatch(node.default_expr);
        walk(static_cast<const Declaration &>(node));
    }

    // ---------------------------------------------------------------------
    // Structural clauses (Entity / Architecture)
    // ---------------------------------------------------------------------
    void visit(const GenericClause &node) override { dispatchAll(node.generics); }

    void visit(const PortClause &node) override { dispatchAll(node.ports); }

    void visit(const Entity &node) override
    {
        dispatch(node.generic_clause);
        dispatch(node.port_clause);
        dispatchAll(node.decls);
        dispatchAll(node.stmts);
    }

    void visit(const Architecture &node) override
    {
        dispatchAll(node.decls);
        dispatchAll(node.stmts);
    }

    // ---------------------------------------------------------------------
    // Statements
    // ---------------------------------------------------------------------
    void visit(const Statement &node) override
    {
        (void)node; // abstract
    }

    void visit(const ConcurrentAssign &node) override
    {
        dispatch(node.target);
        dispatch(node.value);
        walk(static_cast<const Statement &>(node));
    }

    void visit(const Process &node) override
    {
        dispatchAll(node.body);
        walk(static_cast<const Statement &>(node));
    }

    // ---------------------------------------------------------------------
    // Expressions
    // ---------------------------------------------------------------------
    void visit(const Expr &node) override
    {
        (void)node; // abstract
    }

    void visit(const TokenExpr &node) override
    {
        (void)node; // leaf node, nothing to traverse
    }

    void visit(const GroupExpr &node) override
    {
        dispatchAll(node.children);
        walk(static_cast<const Expr &>(node));
    }

    // ---------------------------------------------------------------------
    // Ranges
    // ---------------------------------------------------------------------
    void visit(const Range &node) override
    {
        dispatch(node.left);
        dispatch(node.right);
    }
};

} // namespace ast

#endif /* AST_VISITOR_BASE_HPP */
