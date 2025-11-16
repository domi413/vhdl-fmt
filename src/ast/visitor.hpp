#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

#include "node.hpp"
#include "nodes/design_file.hpp"

#include <concepts>
#include <ranges>
#include <type_traits>
#include <variant>
#include <vector>

namespace ast {

/// @brief Concept for types that can be treated as nullable wrappers
template<typename T>
concept NullableWrapper = requires(const T &wrapper) {
    { *wrapper };                                             // Can dereference
    { wrapper.operator bool() } -> std::convertible_to<bool>; // Can check if null
};

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
    void visit(const ast::DesignFile &design) const { visit(design.units); }

  protected:
    /// @brief Visit a variant node, dispatching to the appropriate operator()
    template<typename... Ts>
    auto visit(const std::variant<Ts...> &node) const -> ReturnType
    {
        return std::visit([this](const auto &n) -> ReturnType { return derived()(n); }, node);
    }

    /// @brief Visit an optional or nullable wrapper (optional<T>, unique_ptr<T>)
    template<NullableWrapper T>
    auto visit(const T &wrapper) const -> ReturnType
    {
        if constexpr (std::is_void_v<ReturnType>) {
            if (wrapper) {
                visit(*wrapper);
            }
        } else {
            return wrapper ? visit(*wrapper) : ReturnType{};
        }
    }

    /// @brief Visit a concrete node (delegates to derived class operator())
    template<typename T>
        requires std::is_base_of_v<NodeBase, T>
    auto visit(const T &node) const -> ReturnType
    {
        return derived()(node);
    }

    /// @brief Visit a vector of nodes
    template<typename T>
    auto visit(const std::vector<T> &nodes) const
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
    auto derived() const noexcept -> const Derived & { return static_cast<const Derived &>(*this); }
};

} // namespace ast

#endif /* AST_VISITOR_HPP */
