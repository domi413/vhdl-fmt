#ifndef EMIT_DEBUG_PRINTER_HPP
#define EMIT_DEBUG_PRINTER_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/visitor_base.hpp"

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace emit {

/// @brief AST debug printer. Traverses the AST and prints its structure with indentation.
struct DebugPrinter : ast::BaseVisitor
{
    explicit DebugPrinter(std::ostream &out) : out_(out) {}

    // Node visitors
    void visit(const ast::DesignFile &node) override;
    void visit(const ast::Entity &node) override;
    void visit(const ast::GenericParam &node) override;
    void visit(const ast::Port &node) override;
    void visit(const ast::Range &node) override;

  private:
    std::ostream &out_;
    std::uint8_t indent_{ 0 };

    void printIndent() const;
    void printLine(std::string_view s) const;

    /// Print node header: `Name [extra] <(N[\n])>`.
    void printNodeHeader(const ast::Node &n,
                         const std::string &extra,
                         std::string_view name_override,
                         std::size_t trailing_breaks) const;

    /// Print only comment lines from a trivia sequence (prefix each line).
    void printCommentLines(const std::vector<ast::Trivia> &tv, std::string_view prefix) const;

    /// Count total trailing newline breaks.
    [[nodiscard]]
    static auto countNewlines(const std::vector<ast::Trivia> &trailing) -> std::size_t;

    template<class NodeT>
    void emitNodeLike(const NodeT &node, std::string_view pretty_name, const std::string &extra);

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
