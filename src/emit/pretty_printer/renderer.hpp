#ifndef EMIT_RENDERER_HPP
#define EMIT_RENDERER_HPP

#include "emit/pretty_printer/doc_impl.hpp"

#include <cstdint>
#include <string>
#include <string_view>

namespace emit {

/// Rendering mode for layout algorithm
enum class Mode : std::uint8_t
{
    FLAT,
    BREAK
};

/// Renderer for the pretty printer
class Renderer final
{
  private:
    int width_;
    int column_{ 0 };
    std::string output_;

  public:
    explicit Renderer(int width) : width_(width) {}

    // Core rendering function
    auto render(const DocPtr &doc) -> std::string;

  private:
    // Internal rendering using visitor pattern
    void renderDoc(int indent, Mode mode, const DocPtr &doc);

    // Check if document fits on current line
    static auto fits(int width, const DocPtr &doc) -> bool;

    // Helper for fits: returns remaining width, or -1 if doesn't fit
    static auto fitsImpl(int width, const DocPtr &doc) -> int;

    // Output helpers
    void write(std::string_view text);
    void newline(int indent);
};

} // namespace emit

#endif // EMIT_RENDERER_HPP
