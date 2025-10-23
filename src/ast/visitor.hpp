#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

#include "nodes/design_file.hpp"

#include <memory>
#include <variant>
#include <vector>

namespace ast {

/// @brief Base class for stateful visitors that need to traverse the AST
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
        return std::visit(
          [this](const auto &n) -> decltype(auto) { return static_cast<Derived &>(*this)(n); },
          node);
    }

    /// @brief Visit a unique_ptr to a variant node
    template<typename... Ts>
    auto visit(const std::unique_ptr<std::variant<Ts...>> &node) -> ReturnType
    {
        if (node) {
            return visit(*node);
        }
        return ReturnType{}; // default-constructed value if null
    }

    /// @brief Visit a unique_ptr to a non-variant type
    template<typename T>
    auto visit(const std::unique_ptr<T> &node) -> decltype(auto)
    {
        if (node) {
            return static_cast<Derived &>(*this)(*node);
        }
    }

    /// @brief Visit a vector of nodes
    template<typename T>
    auto visit(const std::vector<T> &nodes) -> std::vector<ReturnType>
        requires(!std::is_void_v<ReturnType>)
    {
        std::vector<ReturnType> results;
        results.reserve(nodes.size());
        for (const auto &node : nodes) {
            results.push_back(visit(node));
        }
        return results;
    }

    /// @brief Visit a vector of nodes when ReturnType is void
    template<typename T>
    void visit(const std::vector<T> &nodes)
        requires(std::is_void_v<ReturnType>)
    {
        for (const auto &node : nodes) {
            visit(node);
        }
    }
};

/// @brief Type-trait to check if a variant contains a specific type
template<typename T, typename Variant>
struct variant_contains;

template<typename T, typename... Ts>
struct variant_contains<T, std::variant<Ts...>> : std::disjunction<std::is_same<T, Ts>...>
{};

template<typename T, typename Variant>
inline constexpr bool VARIANT_CONTAINS_V = variant_contains<T, Variant>::value;

} // namespace ast

#endif /* AST_VISITOR_HPP */