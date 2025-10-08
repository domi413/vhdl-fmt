#include "debug_printer.hpp"

#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"

#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <typeinfo>

namespace emit {

void DebugPrinter::printIndent() const
{
    std::cout << std::string(static_cast<std::uint8_t>(this->indent * 2), ' ');
}

void DebugPrinter::printNode(const ast::Node &n,
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

void DebugPrinter::visit(const ast::DesignFile &df)
{
    printNode(df, {}, "DesignFile");
    ++this->indent;
    for (const auto &u : df.units) {
        u->accept(*this);
    }
    --this->indent;
}

void DebugPrinter::visit(const ast::Entity &e)
{
    printNode(e, e.name, "Entity");
    ++this->indent;

    // Print generics first
    for (const auto &g : e.generics) {
        g->accept(*this);
    }

    // Then ports
    for (const auto &p : e.ports) {
        p->accept(*this);
    }

    --this->indent;
}

void DebugPrinter::visit(const ast::GenericParam &g)
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

void DebugPrinter::visit(const ast::Port &p)
{
    std::ostringstream oss;

    // Names
    for (const auto &[i, name] : p.names | std::views::enumerate) {
        if (i > 0) {
            oss << ", ";
        }
        oss << name;
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

    ++this->indent;
    for (const auto &r : p.constraints) {
        r->accept(*this);
    }
    --this->indent;
}

void DebugPrinter::visit(const ast::Range &r)
{
    printIndent();
    std::cout << "Range [" << r.left_expr << " " << r.direction << " " << r.right_expr << "]\n";
}

} // namespace emit
