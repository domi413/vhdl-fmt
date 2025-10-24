#ifndef EMIT_DEBUG_PRINTER_HPP
#define EMIT_DEBUG_PRINTER_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/nodes/statements.hpp"
#include "ast/visitor.hpp"

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <string_view>
#include <vector>

namespace emit {

/// @brief AST debug printer. Traverses the AST and prints its structure with indentation.
class DebugPrinter final : public ast::VisitorBase<DebugPrinter, void>
{
  public:
    explicit DebugPrinter(std::ostream &out) : out_(out) {}

    DebugPrinter(const DebugPrinter &) = delete;
    auto operator=(const DebugPrinter &) -> DebugPrinter & = delete;
    DebugPrinter(DebugPrinter &&) = delete;
    auto operator=(DebugPrinter &&) -> DebugPrinter & = delete;
    ~DebugPrinter() = default;

    // Node visitors
    void operator()(const ast::DesignFile &node);
    void operator()(const ast::Entity &node);
    void operator()(const ast::Architecture &node);
    void operator()(const ast::GenericClause &node);
    void operator()(const ast::PortClause &node);
    void operator()(const ast::GenericParam &node);
    void operator()(const ast::Port &node);

    // Declarations
    void operator()(const ast::SignalDecl &node);
    void operator()(const ast::ConstantDecl &node);

    // Expressions
    void operator()(const ast::TokenExpr &node);
    void operator()(const ast::GroupExpr &node);
    void operator()(const ast::UnaryExpr &node);
    void operator()(const ast::BinaryExpr &node);
    void operator()(const ast::ParenExpr &node);

    // Concurrent Statements
    void operator()(const ast::ConcurrentAssign &node);
    void operator()(const ast::Process &node);

    // Sequential Statements
    void operator()(const ast::SequentialAssign &node);
    void operator()(const ast::IfStatement &node);
    void operator()(const ast::CaseStatement &node);
    void operator()(const ast::ForLoop &node);
    void operator()(const ast::WhileLoop &node);

  private:
    std::ostream &out_;
    std::uint8_t indent_{ 0 };

    void printIndent() const;
    void printLine(std::string_view s) const;

    void printNodeHeader(const ast::NodeBase &n,
                         const std::string &extra,
                         std::string_view name_override,
                         std::size_t leading_breaks) const;

    void printTriviaStream(const std::vector<ast::Trivia> &trivia, std::string_view prefix) const;

    void printCommentLines(const std::vector<ast::Comments> &comments,
                           std::string_view prefix) const;

    template<class NodeT>
    void emitNodeLike(const NodeT &node, std::string_view pretty_name, const std::string &extra)
    {
        printNodeHeader(node, extra, pretty_name, 0);

        const IndentGuard _{ indent_ };

        // Print leading trivia stream (newlines + comments in order)
        if (node.trivia && !node.trivia->leading.empty()) {
            printTriviaStream(node.trivia->leading, "↓");
        }

        // Print trailing comments (inline comments)
        if (node.trivia && !node.trivia->trailing.empty()) {
            printCommentLines(node.trivia->trailing, "→");
        }
    }

    struct IndentGuard
    {
        explicit IndentGuard(std::uint8_t &r) : ref_(r) { ++ref_; }
        ~IndentGuard() { --ref_; }
        IndentGuard(const IndentGuard &) = delete;
        auto operator=(const IndentGuard &) -> IndentGuard & = delete;
        IndentGuard(IndentGuard &&) = delete;
        auto operator=(IndentGuard &&) -> IndentGuard & = delete;

      private:
        std::uint8_t &ref_;
    };
};

} // namespace emit

#endif /* EMIT_DEBUG_PRINTER_HPP */
