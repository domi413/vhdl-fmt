#ifndef EMIT_DEBUG_PRINTER_HPP
#define EMIT_DEBUG_PRINTER_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/visitor_base.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace emit {

/// @brief AST debug printer.
/// Traverses the AST and prints its structure with indentation.
struct DebugPrinter : ast::BaseVisitor
{
    void printIndent() const;

    /// Print node header: `Name [extra] <inline_suffix>`
    void printNode(const ast::Node &n,
                   const std::string &extra,
                   const std::string &name_override,
                   const std::string &inline_suffix) const;

    /// Print a trivia sequence (comments as lines; newlines as actual blank lines).
    void printTriviaLines(const std::vector<ast::Trivia> &tv) const;

    /// Build the inline suffix from trailing trivia:
    ///   "<c1> <c2> ... (N[\n])"
    /// where N is the total trailing newline breaks (omitted if 0).
    [[nodiscard]] auto buildInlineSuffix(const std::vector<ast::Trivia> &trailing) const
      -> std::string;

    // Node visitors
    void visit(const ast::DesignFile &node) override;
    void visit(const ast::Entity &node) override;
    void visit(const ast::GenericParam &node) override;
    void visit(const ast::Port &node) override;
    void visit(const ast::Range &node) override;

  private:
    template<class NodeT>
    void emitNodeLike(const NodeT &node, std::string_view pretty_name, const std::string &extra);

    std::uint8_t indent{ 0 };
};

} // namespace emit

#endif /* EMIT_DEBUG_PRINTER_HPP */
