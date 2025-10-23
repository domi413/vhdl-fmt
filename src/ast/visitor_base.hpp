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
///
/// @tparam ReturnType The type returned by visit methods (void by default)
template<typename ReturnType = void>
class BaseVisitor : public Visitor<ReturnType>
{
  protected:
    /// @brief Safely visit a node if it exists.
    template<AstNode T>
    auto dispatch(const std::unique_ptr<T> &node) -> ReturnType
    {
        if (node) {
            if constexpr (std::is_void_v<ReturnType>) {
                node->accept(*this);
            } else {
                return node->template accept<ReturnType>(*this);
            }
        }
        if constexpr (!std::is_void_v<ReturnType>) {
            return ReturnType{};
        }
    }

    /// @brief Visit all elements of a vector of nodes.
    template<AstNode T>
    auto dispatchAll(const std::vector<std::unique_ptr<T>> &vec) -> ReturnType
    {
        if constexpr (std::is_void_v<ReturnType>) {
            for (const auto &node : vec) {
                dispatch(node);
            }
        } else {
            ReturnType result{};
            for (const auto &node : vec) {
                result = dispatch(node);
            }
            return result;
        }
    }

    /// @brief Continue default traversal manually.
    template<AstNode T>
    auto walk(const T &node) -> ReturnType
    {
        return this->BaseVisitor::visit(node);
    }

  public:
    // ---------------------------------------------------------------------
    // Core hierarchy
    // ---------------------------------------------------------------------
    auto visit(const Node &node) -> ReturnType override
    {
        (void)node; // abstract root, nothing to traverse
        if constexpr (!std::is_void_v<ReturnType>) {
            return ReturnType{};
        }
    }

    auto visit(const DesignFile &node) -> ReturnType override
    {
        return dispatchAll(node.units);
    }

    auto visit(const DesignUnit &node) -> ReturnType override
    {
        (void)node; // abstract
        if constexpr (!std::is_void_v<ReturnType>) {
            return ReturnType{};
        }
    }

    // ---------------------------------------------------------------------
    // Declarations
    // ---------------------------------------------------------------------
    auto visit(const Declaration &node) -> ReturnType override
    {
        (void)node; // abstract
        if constexpr (!std::is_void_v<ReturnType>) {
            return ReturnType{};
        }
    }

    auto visit(const ConstantDecl &node) -> ReturnType override
    {
        dispatch(node.init_expr);
        return walk(static_cast<const Declaration &>(node));
    }

    auto visit(const SignalDecl &node) -> ReturnType override
    {
        dispatchAll(node.constraints);
        dispatch(node.init_expr);
        return walk(static_cast<const Declaration &>(node));
    }

    auto visit(const GenericParam &node) -> ReturnType override
    {
        dispatch(node.default_expr);
        return walk(static_cast<const Declaration &>(node));
    }

    auto visit(const Port &node) -> ReturnType override
    {
        dispatchAll(node.constraints);
        dispatch(node.default_expr);
        return walk(static_cast<const Declaration &>(node));
    }

    // ---------------------------------------------------------------------
    // Structural clauses (Entity / Architecture)
    // ---------------------------------------------------------------------
    auto visit(const GenericClause &node) -> ReturnType override
    {
        return dispatchAll(node.generics);
    }

    auto visit(const PortClause &node) -> ReturnType override
    {
        return dispatchAll(node.ports);
    }

    auto visit(const Entity &node) -> ReturnType override
    {
        dispatch(node.generic_clause);
        dispatch(node.port_clause);
        dispatchAll(node.decls);
        return dispatchAll(node.stmts);
    }

    auto visit(const Architecture &node) -> ReturnType override
    {
        dispatchAll(node.decls);
        return dispatchAll(node.stmts);
    }

    // ---------------------------------------------------------------------
    // Statements
    // ---------------------------------------------------------------------
    auto visit(const Statement &node) -> ReturnType override
    {
        (void)node; // abstract
        if constexpr (!std::is_void_v<ReturnType>) {
            return ReturnType{};
        }
    }

    auto visit(const ConcurrentAssign &node) -> ReturnType override
    {
        dispatch(node.target);
        dispatch(node.value);
        return walk(static_cast<const Statement &>(node));
    }

    auto visit(const Process &node) -> ReturnType override
    {
        dispatchAll(node.body);
        return walk(static_cast<const Statement &>(node));
    }

    // ---------------------------------------------------------------------
    // Expressions
    // ---------------------------------------------------------------------
    auto visit(const Expr &node) -> ReturnType override
    {
        (void)node; // abstract
        if constexpr (!std::is_void_v<ReturnType>) {
            return ReturnType{};
        }
    }

    auto visit(const TokenExpr &node) -> ReturnType override
    {
        (void)node; // leaf node, nothing to traverse
        if constexpr (!std::is_void_v<ReturnType>) {
            return ReturnType{};
        }
    }

    auto visit(const GroupExpr &node) -> ReturnType override
    {
        dispatchAll(node.children);
        return walk(static_cast<const Expr &>(node));
    }

    auto visit(const UnaryExpr &node) -> ReturnType override
    {
        dispatch(node.value);
        return walk(static_cast<const Expr &>(node));
    }

    auto visit(const BinaryExpr &node) -> ReturnType override
    {
        dispatch(node.left);
        dispatch(node.right);
        return walk(static_cast<const Expr &>(node));
    }

    auto visit(const ParenExpr &node) -> ReturnType override
    {
        dispatch(node.inner);
        return walk(static_cast<const Expr &>(node));
    }
};

} // namespace ast

#endif /* AST_VISITOR_BASE_HPP */
