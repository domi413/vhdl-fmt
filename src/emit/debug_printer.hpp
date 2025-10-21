#ifndef EMIT_DEBUG_PRINTER_HPP
#define EMIT_DEBUG_PRINTER_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/entity.hpp"
#include "ast/nodes/expressions.hpp"
#include "ast/visitor_base.hpp"

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <string_view>
#include <vector>

namespace emit {

/// @brief AST debug printer. Traverses the AST and prints its structure with indentation.
class DebugPrinter : public ast::BaseVisitor
{
  public:
    explicit DebugPrinter(std::ostream &out) : out_(out) {}

    // Node visitors
    void visit(const ast::DesignFile &node) override;
    void visit(const ast::Entity &node) override;
    void visit(const ast::Architecture &node) override;
    void visit(const ast::GenericClause &node) override;
    void visit(const ast::PortClause &node) override;
    void visit(const ast::GenericParam &node) override;
    void visit(const ast::Port &node) override;

    // Declarations
    void visit(const ast::SignalDecl &node) override;
    void visit(const ast::ConstantDecl &node) override;

    // Expressions
    void visit(const ast::TokenExpr &node) override;
    void visit(const ast::GroupExpr &node) override;
    void visit(const ast::UnaryExpr &node) override;
    void visit(const ast::BinaryExpr &node) override;
    void visit(const ast::ParenExpr &node) override;

  private:
    std::ostream &out_;
    std::uint8_t indent_{ 0 };

    void printIndent() const;
    void printLine(std::string_view s) const;

    void printNodeHeader(const ast::Node &n,
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
        if (!node.leading().empty()) {
            printTriviaStream(node.leading(), "↓");
        }

        // Print trailing comments (inline comments)
        if (!node.trailing().empty()) {
            printCommentLines(node.trailing(), "→");
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
