#include "emit/pretty_printer/renderer.hpp"

#include "common/config.hpp"
#include "emit/pretty_printer/doc_impl.hpp"

#include <string>
#include <type_traits>
#include <variant>

namespace emit {

Renderer::Renderer(const common::Config &config) :
  width_(config.line_config.line_length),
  indent_size_(config.line_config.indent_size),
  align_(config.port_map.align_signals)
{
}

auto Renderer::render(const DocPtr &doc) -> std::string
{
    output_.clear();
    column_ = 0;

    renderDoc(0, Mode::BREAK, doc);

    return output_;
}

void Renderer::renderDoc(int indent, Mode mode, const DocPtr &doc)
{
    if (!doc) {
        return;
    }

    std::visit(
      [&](const auto &node) -> void {
          using T = std::decay_t<decltype(node)>;

          // Dispatch logic using if constexpr
          if constexpr (std::is_same_v<T, Empty>) {
              // Empty produces nothing
          } else if constexpr (std::is_same_v<T, Text>) {
              write(node.content);
          } else if constexpr (std::is_same_v<T, SoftLine>) {
              if (mode == Mode::FLAT) {
                  // In flat mode, line becomes space
                  write(" ");
              } else {
                  // In break mode, line becomes newline + indent
                  newline(indent);
              }
          } else if constexpr (std::is_same_v<T, HardLine>) {
              // Hard line always breaks
              newline(indent);
          } else if constexpr (std::is_same_v<T, Concat>) {
              renderDoc(indent, mode, node.left);
              renderDoc(indent, mode, node.right);
          } else if constexpr (std::is_same_v<T, Nest>) {
              // Increase indentation for nested content
              renderDoc(indent + indent_size_, mode, node.doc);
          } else if constexpr (std::is_same_v<T, Align>) {
              DocPtr doc_to_render = node.doc;
              if (align_) {
                  // Run the two-pass logic to resolve alignment
                  doc_to_render = resolveAlignment(node.doc);
              }

              // Render the (possibly) aligned inner document
              renderDoc(indent, mode, doc_to_render);
          } else if constexpr (std::is_same_v<T, AlignPlaceholder>) {
              // In non-aligned mode - treat as text
              write(node.content);
          } else if constexpr (std::is_same_v<T, Union>) {
              // Decide: use flat or broken layout?
              if (mode == Mode::FLAT || fits(width_ - column_, node.flat)) {
                  // Fits on current line - use flat version
                  renderDoc(indent, Mode::FLAT, node.flat);
              } else {
                  // Doesn't fit - use broken version
                  renderDoc(indent, Mode::BREAK, node.broken);
              }
          }
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

    // Use a single generic lambda to replace common::Overload
    return std::visit(
      [&](const auto &node) -> int {
          using T = std::decay_t<decltype(node)>;

          // Dispatch logic using if constexpr
          if constexpr (std::is_same_v<T, Empty>) {
              return width;
          } else if constexpr (std::is_same_v<T, Text>) {
              return width - static_cast<int>(node.content.length());
          } else if constexpr (std::is_same_v<T, SoftLine>) {
              // In flat mode, line becomes space
              return width - 1;
          } else if constexpr (std::is_same_v<T, Concat>) {
              // Thread remaining width through both sides
              const int remaining = fitsImpl(width, node.left);
              if (remaining < 0) {
                  return -1;
              }
              return fitsImpl(remaining, node.right);
          } else if constexpr (std::is_same_v<T, Nest>) {
              // Nest doesn't consume width
              return fitsImpl(width, node.doc);
          } else if constexpr (std::is_same_v<T, Union>) {
              // Check flat version only
              return fitsImpl(width, node.flat);
          } else if constexpr (std::is_same_v<T, AlignPlaceholder>) {
              // AlignPlaceholder acts like text
              return width - static_cast<int>(node.content.length());
          } else if constexpr (std::is_same_v<T, Align>) {
              // Align node just wraps an inner doc
              return fitsImpl(width, node.doc);
          }
          // All others (like HardLine) do not fit
          else {
              return -1;
          }
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
