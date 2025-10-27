#include "ast/node.hpp"
#include "emit/debug_printer.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
#include <string_view>
#include <typeinfo>
#include <variant>

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

void DebugPrinter::printNodeHeader(const ast::NodeBase &n,
                                   std::string_view extra,
                                   std::string_view name_override,
                                   const std::size_t leading_breaks) const
{
    printIndent();
    out_ << (!name_override.empty() ? std::string_view{ name_override } : typeid(n).name());

    if (!extra.empty()) {
        out_ << " [" << extra << "]";
    }

    if (leading_breaks > 0U) {
        out_ << " (" << leading_breaks << R"([\n]))";
    }

    out_ << '\n';
}

void DebugPrinter::printTriviaStream(std::span<const ast::Trivia> trivia,
                                     std::string_view prefix) const
{
    for (const auto &t : trivia) {
        if (const auto *pb = std::get_if<ast::ParagraphBreak>(&t)) {
            printIndent();
            out_
              << prefix
              << " [paragraph break: "
              << pb->blank_lines
              << (pb->blank_lines == 1 ? " blank line]" : " blank lines]")
              << '\n';
        } else if (const auto *c = std::get_if<ast::Comments>(&t)) {
            printIndent();
            out_ << prefix << " " << c->text << '\n';
        }
    }
}

void DebugPrinter::printCommentLines(std::span<const ast::Comments> comments,
                                     std::string_view prefix) const
{
    for (const auto &comment : comments) {
        printIndent();

        if (!prefix.empty()) {
            out_ << prefix << " ";
        }

        out_ << comment.text << '\n';
    }
}

} // namespace emit
