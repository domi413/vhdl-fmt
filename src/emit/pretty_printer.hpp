#ifndef EMIT_PRETTY_PRINTER_HPP
#define EMIT_PRETTY_PRINTER_HPP

#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "ast/visitor.hpp"
#include "common/config.hpp"
#include "emit/pretty_printer/doc.hpp"

namespace emit {

class PrettyPrinter final : public ast::VisitorBase<PrettyPrinter, Doc>
{
  public:
    PrettyPrinter() = default;
    explicit PrettyPrinter(common::Config config) : config_(config) {}

    PrettyPrinter(const PrettyPrinter &) = delete;
    auto operator=(const PrettyPrinter &) -> PrettyPrinter & = delete;
    PrettyPrinter(PrettyPrinter &&) = delete;
    auto operator=(PrettyPrinter &&) -> PrettyPrinter & = delete;
    ~PrettyPrinter() = default;

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

  protected:
    // Config accessor for visitor methods
    [[nodiscard]]
    auto config() const -> const common::Config &
    {
        return config_;
    }

  private:
    common::Config config_;
};

} // namespace emit

#endif // EMIT_PRETTY_PRINTER_HPP
