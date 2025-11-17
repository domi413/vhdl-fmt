#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

#include "node.hpp"

#include <ranges>
#include <type_traits>
#include <variant>
#include <vector>

namespace ast {

/// @brief Base class for stateful visitors that need to traverse the AST
///
/// Implements CRTP pattern for compile-time polymorphism. Derived classes
/// must implement operator() for each concrete AST node type they handle.
///
/// Example usage:
///   struct MyFormatter : VisitorBase<MyFormatter> {
///       std::string output;
///
///       void operator()(const TokenExpr& e) {
///           output += e.text;
///       }
///   };
template<typename Derived, typename ReturnType = void>
class VisitorBase
{
  public:
    /// @brief Visit a concrete node (delegates to derived class operator())
    template<typename T>
        requires std::is_base_of_v<NodeBase, T>
    auto visit(const T &node) const -> ReturnType
    {
        if constexpr (std::is_void_v<ReturnType>) {
            return derived()(node);
        } else {
            return derived().wrapResult(node, derived()(node));
        }
    }

    /// @brief Visit a variant node, rerouting to `visit(const T &node)`
    template<typename... Ts>
    auto visit(const std::variant<Ts...> &node) const -> ReturnType
    {
        return std::visit([this](const auto &n) -> ReturnType { return derived().visit(n); }, node);
    }

    /// @brief Visit a vector of nodes, rerouting to `visit(const T &node)`
    template<typename T>
    auto visit(const std::vector<T> &nodes) const
      -> std::conditional_t<std::is_void_v<ReturnType>, void, std::vector<ReturnType>>
    {
        if constexpr (std::is_void_v<ReturnType>) {
            for (const auto &node : nodes) {
                derived().visit(node);
            }
        } else {
            return nodes
                 | std::views::transform([this](const T &n) { return visit(n); })
                 | std::ranges::to<std::vector>();
        }
    }

  protected:
    template<typename T>
    auto wrapResult(const T & /*node*/, ReturnType result) const -> ReturnType
    {
        return result; // Default implementation
    }

  private:
    auto derived() const noexcept -> const Derived & { return static_cast<const Derived &>(*this); }
};

} // namespace ast

#endif /* AST_VISITOR_HPP */
