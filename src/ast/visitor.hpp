#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

#include "node.hpp"
#include "nodes/design_file.hpp"

#include <memory>
#include <optional>
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
    /// @brief Visit a DesignFile by visiting all its units
    void visit(const ast::DesignFile &design) { visit(design.units); }

  protected:
    /// @brief Visit a variant node, dispatching to the appropriate operator()
    template<typename... Ts>
    auto visit(const std::variant<Ts...> &node) -> ReturnType
    {
        return std::visit([this](const auto &n) -> ReturnType { return derived()(n); }, node);
    }

    /// @brief Visit a Box<T> (unique_ptr), calling operator() if non-null
    template<typename T>
    auto visit(const std::unique_ptr<T> &node) -> ReturnType
    {
        if constexpr (std::is_void_v<ReturnType>) {
            if (node) {
                visit(*node);
            }
        } else {
            return node ? visit(*node) : ReturnType{};
        }
    }

    /// @brief Visit an optional value, calling operator() if present
    template<typename T>
    auto visit(const std::optional<T> &node) -> ReturnType
    {
        if constexpr (std::is_void_v<ReturnType>) {
            if (node) {
                visit(*node);
            }
        } else {
            return node ? visit(*node) : ReturnType{};
        }
    }

    /// @brief Visit a concrete node (delegates to derived class operator())
    template<typename T>
        requires std::is_base_of_v<NodeBase, T>
    auto visit(const T &node) -> ReturnType
    {
        return derived()(node);
    }

    /// @brief Visit a vector of nodes
    template<typename T>
    auto visit(const std::vector<T> &nodes)
      -> std::conditional_t<std::is_void_v<ReturnType>, void, std::vector<ReturnType>>
    {
        if constexpr (std::is_void_v<ReturnType>) {
            for (const auto &node : nodes) {
                visit(node);
            }
        } else {
            return nodes
                 | std::views::transform([this](const T &n) { return visit(n); })
                 | std::ranges::to<std::vector>();
        }
    }

  private:
    auto derived() -> Derived & { return static_cast<Derived &>(*this); }
};

} // namespace ast

#endif /* AST_VISITOR_HPP */