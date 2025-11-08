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
  private:
    common::Config config_;

  protected:
    // Config accessor for visitor methods
    [[nodiscard]] auto config() const -> const common::Config& { return config_; }

  public:
    PrettyPrinter() = default;
    explicit PrettyPrinter(common::Config config) : config_(config) {}

    PrettyPrinter(const PrettyPrinter &) = delete;
    auto operator=(const PrettyPrinter &) -> PrettyPrinter & = delete;
    PrettyPrinter(PrettyPrinter &&) = delete;
    auto operator=(PrettyPrinter &&) -> PrettyPrinter & = delete;
    ~PrettyPrinter() = default;

    // Node visitors
    auto operator()(const ast::DesignFile &node) -> Doc;
    auto operator()(const ast::Entity &node) -> Doc;
    auto operator()(const ast::Architecture &node) -> Doc;
    auto operator()(const ast::GenericClause &node) -> Doc;
    auto operator()(const ast::PortClause &node) -> Doc;
    auto operator()(const ast::GenericParam &node) -> Doc;
    auto operator()(const ast::Port &node) -> Doc;

    // Declarations
    auto operator()(const ast::SignalDecl &node) -> Doc;
    auto operator()(const ast::ConstantDecl &node) -> Doc;

    // Expressions
    auto operator()(const ast::TokenExpr &node) -> Doc;
    auto operator()(const ast::GroupExpr &node) -> Doc;
    auto operator()(const ast::UnaryExpr &node) -> Doc;
    auto operator()(const ast::BinaryExpr &node) -> Doc;
    auto operator()(const ast::ParenExpr &node) -> Doc;
    auto operator()(const ast::CallExpr &node) -> Doc;

    // Constraints
    auto operator()(const ast::IndexConstraint &node) -> Doc;
    auto operator()(const ast::RangeConstraint &node) -> Doc;

    // Concurrent Statements
    auto operator()(const ast::ConcurrentAssign &node) -> Doc;
    auto operator()(const ast::Process &node) -> Doc;

    // Sequential Statements
    auto operator()(const ast::SequentialAssign &node) -> Doc;
    auto operator()(const ast::IfStatement &node) -> Doc;
    auto operator()(const ast::CaseStatement &node) -> Doc;
    auto operator()(const ast::ForLoop &node) -> Doc;
    auto operator()(const ast::WhileLoop &node) -> Doc;
};

} // namespace emit

#endif // EMIT_PRETTY_PRINTER_HPP
