#ifndef EMIT_PRETTY_PRINTER_HPP
#define EMIT_PRETTY_PRINTER_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "ast/visitor.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <functional>
#include <ranges>
#include <vector>

namespace emit {

class PrettyPrinter final : public ast::VisitorBase<PrettyPrinter, Doc>
{
  public:
    // Node visitors
    auto operator()(const ast::DesignFile &node) const -> Doc;
    auto operator()(const ast::Entity &node) const -> Doc;
    auto operator()(const ast::Architecture &node) const -> Doc;
    auto operator()(const ast::GenericClause &node) const -> Doc;
    auto operator()(const ast::PortClause &node) const -> Doc;
    auto operator()(const ast::GenericParam &node) const -> Doc;
    auto operator()(const ast::Port &node) const -> Doc;

    // Declarations
    auto operator()(const ast::SignalDecl &node) const -> Doc;
    auto operator()(const ast::ConstantDecl &node) const -> Doc;

    // Expressions
    auto operator()(const ast::TokenExpr &node) const -> Doc;
    auto operator()(const ast::GroupExpr &node) const -> Doc;
    auto operator()(const ast::UnaryExpr &node) const -> Doc;
    auto operator()(const ast::BinaryExpr &node) const -> Doc;
    auto operator()(const ast::ParenExpr &node) const -> Doc;
    auto operator()(const ast::CallExpr &node) const -> Doc;

    // Constraints
    auto operator()(const ast::IndexConstraint &node) const -> Doc;
    auto operator()(const ast::RangeConstraint &node) const -> Doc;

    // Concurrent Statements
    auto operator()(const ast::ConcurrentAssign &node) const -> Doc;
    auto operator()(const ast::Process &node) const -> Doc;

    // Sequential Statements
    auto operator()(const ast::SequentialAssign &node) const -> Doc;
    auto operator()(const ast::IfStatement &node) const -> Doc;
    auto operator()(const ast::CaseStatement &node) const -> Doc;
    auto operator()(const ast::ForLoop &node) const -> Doc;
    auto operator()(const ast::WhileLoop &node) const -> Doc;

  private:
    /// @brief This is the main wrapper. It combines the core doc with trivia.
    [[nodiscard]]
    static auto withTrivia(const ast::NodeBase &node, Doc core_doc) -> Doc;

    /// @brief Prints a generic parameter declaration.
    [[nodiscard]]
    auto printGenericParam(const ast::GenericParam &node, bool is_last) const -> Doc;

    /// @brief Prints a port declaration.
    [[nodiscard]]
    auto printPort(const ast::Port &node, bool is_last) const -> Doc;

    template<std::ranges::range Range, typename PrintHelper>
    auto printList(Range &items, PrintHelper &&print_func) const -> std::vector<Doc>
    {
        std::vector<Doc> results;
        if (std::ranges::empty(items)) {
            return results;
        }

        if constexpr (std::ranges::sized_range<Range>) {
            results.reserve(std::ranges::size(items));
        }

        auto it = std::ranges::begin(items);
        const auto end = std::ranges::end(items);

        while (it != end) {
            const bool is_last = (std::next(it) == end);
            const Doc doc = std::invoke(std::forward<PrintHelper>(print_func), this, *it, is_last);
            results.emplace_back(doc);
            ++it;
        }
        return results;
    }
};

} // namespace emit

#endif // EMIT_PRETTY_PRINTER_HPP
