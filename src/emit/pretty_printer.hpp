#ifndef EMIT_PRETTY_PRINTER_HPP
#define EMIT_PRETTY_PRINTER_HPP

#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "ast/visitor.hpp"
#include "emit/pretty_printer/doc.hpp"

namespace emit {

class PrettyPrinter final : public ast::VisitorBase<PrettyPrinter, Doc>
{
  public:
    PrettyPrinter() = default;

    PrettyPrinter(const PrettyPrinter &) = delete;
    auto operator=(const PrettyPrinter &) -> PrettyPrinter & = delete;
    PrettyPrinter(PrettyPrinter &&) = delete;
    auto operator=(PrettyPrinter &&) -> PrettyPrinter & = delete;
    ~PrettyPrinter() = default;

    // Node visitors
    auto operator()(const ast::DesignFile &node) -> Doc;
    auto operator()(const ast::EntityDecl &node) -> Doc;
    auto operator()(const ast::ArchitectureBody &node) -> Doc;
    auto operator()(const ast::GenericClause &node) -> Doc;
    auto operator()(const ast::PortClause &node) -> Doc;
    auto operator()(const ast::InterfaceConstantDecl &node) -> Doc;
    auto operator()(const ast::InterfacePortDecl &node) -> Doc;

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

    // Concurrent Statements
    auto operator()(const ast::ConcurrentSignalAssignmentStatement &node) -> Doc;
    auto operator()(const ast::ProcessStatement &node) -> Doc;

    // Sequential Statements
    auto operator()(const ast::SignalAssignmentStatement &node) -> Doc;
    auto operator()(const ast::IfStatement &node) -> Doc;
    auto operator()(const ast::CaseStatement &node) -> Doc;
    auto operator()(const ast::ForLoopStatement &node) -> Doc;
    auto operator()(const ast::WhileLoopStatement &node) -> Doc;
};

} // namespace emit

#endif // EMIT_PRETTY_PRINTER_HPP
