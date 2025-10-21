#include "ast/node.hpp"
#include "emit/debug_printer.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <typeinfo>
#include <variant>
#include <vector>

namespace emit {

void DebugPrinter::printIndent() const
{
    out_ << std::string(static_cast<std::uint8_t>(indent_ * 2), ' ');
}

void DebugPrinter::printLine(std::string_view s) const
{
    printIndent();
    out_ << s << '\n';
}

void DebugPrinter::printNodeHeader(const ast::Node &n,
                                   const std::string &extra,
                                   std::string_view name_override,
                                   std::size_t trailing_breaks) const
{
    printIndent();
    out_ << (!name_override.empty() ? std::string{ name_override } : typeid(n).name());
    if (!extra.empty()) {
        out_ << " [" << extra << "]";
    }
    if (trailing_breaks > 0U) {
        out_ << " (" << trailing_breaks << R"([\n]))";
    }
    out_ << '\n';
}

void DebugPrinter::printCommentLines(const std::vector<ast::Comments> &comments,
                                     std::string_view prefix) const
{
    for (const auto &comment : comments) {
        printIndent();
        if (!prefix.empty()) {
            out_ << prefix;
        }
        out_ << comment.text << '\n';
    }
}

auto DebugPrinter::countNewlines(const std::vector<ast::Trivia> &leading) -> std::size_t
{
    std::size_t total = 0;
    for (const auto &t : leading) {
        if (const auto *nl = std::get_if<ast::Newlines>(&t)) {
            total += nl->breaks;
        }
    }
    return total;
}

} // namespace emit