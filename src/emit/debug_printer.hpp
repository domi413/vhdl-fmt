#ifndef EMIT_DEBUG_PRINTER_HPP
#define EMIT_DEBUG_PRINTER_HPP

#include "ast/node.hpp"
#include "ast/visitor.hpp"

#include <string>

namespace emit {

struct DebugPrinter : ast::Visitor
{
    void printIndent() const;
    void printNode(const ast::Node &n,
                   const std::string &extra,
                   const std::string &name_override) const;

    // ---- Node visitors ----
    void visit(const ast::DesignFile &df) override;
    void visit(const ast::Entity &e) override;
    void visit(const ast::GenericParam &g) override;
    void visit(const ast::Port &p) override;
    void visit(const ast::Range &r) override;

  private:
    std::uint8_t indent{ 0 };
};

} // namespace emit

#endif /* EMIT_DEBUG_PRINTER_HPP */
