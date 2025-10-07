#ifndef EMIT_DEBUG_PRINTER_HPP
#define EMIT_DEBUG_PRINTER_HPP

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/visitor.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>

namespace emit {

struct DebugPrinter : ast::ASTVisitor
{
    void printIndent() const
    {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";
        }
    }

    void printNode(const ast::Node &n,
                   const std::string &extra,
                   const std::string &name_override) const
    {
        printIndent();
        if (!name_override.empty()) {
            std::cout << name_override;
        } else {
            std::cout << typeid(n).name(); // fallback RTTI
        }
        if (!extra.empty()) {
            std::cout << " [" << extra << "]";
        }
        std::cout << "\n";
    }

    // ---- Nodes ----
    void visit(const ast::DesignFile &df) override
    {
        printNode(df, {}, "DesignFile");
        indent++;
        for (const auto &u : df.units) {
            u->accept(*this);
        }
        indent--;
    }

    void visit(const ast::Entity &e) override
    {
        printNode(e, e.name, "Entity");
        indent++;

        // Print generics first
        for (const auto &g : e.generics) {
            g->accept(*this);
        }

        // Then ports
        for (const auto &p : e.ports) {
            p->accept(*this);
        }

        indent--;
    }

    void visit(const ast::GenericParam &g) override
    {
        std::string info;
        for (const auto &n : g.names) {
            if (!info.empty()) {
                info += ",";
            }
            info += n;
        }
        info += " : " + g.type;
        if (g.init.has_value()) {
            info += " := " + *g.init;
        }
        printNode(g, info, "Generic");
    }

    void visit(const ast::Port &p) override
    {
        std::ostringstream oss;

        // Names
        for (std::size_t i = 0; i < p.names.size(); ++i) {
            if (i > 0) {
                oss << ", ";
            }
            oss << p.names[i];
        }

        // Mode + Type
        if (!p.mode.empty() || !p.type.empty()) {
            oss << " :";
            if (!p.mode.empty()) {
                oss << " " << p.mode;
            }
            if (!p.type.empty()) {
                oss << " " << p.type;
            }
        }

        printNode(p, oss.str(), "Port");

        indent++;
        for (const auto &r : p.constraints) {
            r->accept(*this);
        }
        indent--;
    }

    void visit(const ast::Range &r) override
    {
        printIndent();
        std::cout << "Range [" << r.left_expr << " " << r.direction << " " << r.right_expr << "]\n";
    }

  private:
    int indent = 0;
};

} // namespace emit

#endif /* EMIT_DEBUG_PRINTER_HPP */
