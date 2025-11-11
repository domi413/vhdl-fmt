#include "emit/pretty_printer/renderer.hpp"

#include "common/config.hpp"
#include "common/overload.hpp"
#include "emit/pretty_printer/doc_impl.hpp"

#include <string>
#include <variant>

namespace emit {

Renderer::Renderer(const common::Config &config) :
  width_(config.line_config.line_length),
  indent_size_(config.line_config.indent_size)
{
}

auto Renderer::render(const DocPtr &doc) -> std::string
{
    output_.clear();
    column_ = 0;

    renderDoc(0, Mode::BREAK, doc);

    return output_;
}

// Internal rendering using visitor pattern with std::visit
void Renderer::renderDoc(int indent, Mode mode, const DocPtr &doc)
{
    if (!doc) {
        return;
    }

    std::visit(common::Overload{
                 [&](const Empty &) -> void {
                     // Empty produces nothing
                 },
                 [&](const Text &node) -> void { write(node.content); },
                 [&](const SoftLine &) -> void {
                     if (mode == Mode::FLAT) {
                         // In flat mode, line becomes space
                         write(" ");
                     } else {
                         // In break mode, line becomes newline + indent
                         newline(indent);
                     }
                 },
                 [&](const HardLine &) -> void {
                     // Hard line always breaks, regardless of mode
                     newline(indent);
                 },
                 [&](const Concat &node) -> void {
                     renderDoc(indent, mode, node.left);
                     renderDoc(indent, mode, node.right);
                 },
                 [&](const Nest &node) -> void {
                     // Increase indentation for nested content
                     renderDoc(indent + indent_size_, mode, node.doc);
                 },
                 [&](const Union &node) -> void {
                     // Decide: use flat or broken layout?
                     if (mode == Mode::FLAT || fits(width_ - column_, node.flat)) {
                         // Fits on current line - use flat version
                         renderDoc(indent, Mode::FLAT, node.flat);
                     } else {
                         // Doesn't fit - use broken version
                         renderDoc(indent, Mode::BREAK, node.broken);
                     }
                 },
               },
               doc->value);
}

// Check if document fits on current line
auto Renderer::fits(int width, const DocPtr &doc) -> bool
{
    return fitsImpl(width, doc) >= 0;
}

// Helper: simulate flattened rendering and return remaining width
auto Renderer::fitsImpl(int width, const DocPtr &doc) -> int
{
    if (!doc) {
        return width;
    }
    if (width < 0) {
        return -1;
    }

    return std::visit(
      common::Overload{
        [&](const Empty &) -> int { return width; },
        [&](const Text &node) -> int { return width - static_cast<int>(node.content.length()); },
        [&](const SoftLine &) -> int {
            // In flat mode, line becomes space
            return width - 1;
        },
        [&](const HardLine &) -> int {
            // Hard line never fits on the same line
            return -1;
        },
        [&](const Concat &node) -> int {
            // Thread remaining width through both sides
            const int remaining = fitsImpl(width, node.left);
            if (remaining < 0) {
                return -1;
            }
            return fitsImpl(remaining, node.right);
        },
        [&](const Nest &node) -> int {
            // Nest doesn't consume width
            return fitsImpl(width, node.doc);
        },
        [&](const Union &node) -> int {
            // Check flat version only
            return fitsImpl(width, node.flat);
        },
      },
      doc->value);
}

// Output helpers
void Renderer::write(std::string_view text)
{
    output_ += text;
    column_ += static_cast<int>(text.length());
}

void Renderer::newline(int indent)
{
    output_ += '\n';
    output_.append(indent, ' ');
    column_ = indent;
}

} // namespace emit
