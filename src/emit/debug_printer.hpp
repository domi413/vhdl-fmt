#ifndef EMIT_DEBUG_PRINTER_HPP
#define EMIT_DEBUG_PRINTER_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/visitor_base.hpp"

#include <cstdint>
#include <string>

namespace emit {

/**
 * @brief AST debug printer.
 *
 * Traverses the AST and prints its structure with indentation.
 * Inherits recursive traversal from `BaseVisitor`.
 */
struct DebugPrinter : ast::BaseVisitor
{
    void printIndent() const;
    void printNode(const ast::Node &n,
                   const std::string &extra,
                   const std::string &name_override) const;

    // Node visitors
    void visit(const ast::DesignFile &node) override;
    void visit(const ast::Entity &node) override;
    void visit(const ast::GenericParam &node) override;
    void visit(const ast::Port &node) override;
    void visit(const ast::Range &node) override;

  private:
    std::uint8_t indent{ 0 };
};

} // namespace emit

#endif /* EMIT_DEBUG_PRINTER_HPP */
