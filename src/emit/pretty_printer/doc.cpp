#include "emit/pretty_printer/doc.hpp"

#include "common/config.hpp"
#include "emit/pretty_printer/doc_impl.hpp"
#include "emit/pretty_printer/renderer.hpp"

#include <string>
#include <string_view>

namespace emit {

Doc::Doc() : impl_(makeEmpty()) {}

// Basic constructors
auto Doc::empty() -> Doc
{
    return Doc(makeEmpty());
}

auto Doc::text(std::string_view str) -> Doc
{
    return Doc(makeText(str));
}

auto Doc::line() -> Doc
{
    return Doc(makeLine());
}

auto Doc::hardline() -> Doc
{
    return Doc(makeHardLine());
}

// Combinators
auto Doc::operator+(const Doc &other) const -> Doc
{
    return Doc(makeConcat(impl_, other.impl_));
}

auto Doc::operator&(const Doc &other) const -> Doc
{
    return *this + space() + other;
}

auto Doc::operator/(const Doc &other) const -> Doc
{
    return *this + line() + other;
}

auto Doc::operator|(const Doc &other) const -> Doc
{
    return *this + hardline() + other;
}

auto Doc::operator<<(const Doc &other) const -> Doc
{
    // *this + (line() + other).nest(DEFAULT_INDENT)
    auto nested = Doc(makeNest(DEFAULT_INDENT, makeConcat(line().impl_, other.impl_)));
    return *this + nested;
}

auto Doc::operator>>(const Doc &other) const -> Doc
{
    // *this + (line() + other).nest(-DEFAULT_INDENT)
    auto nested = Doc(makeNest(-DEFAULT_INDENT, makeConcat(line().impl_, other.impl_)));
    return *this + nested;
}

auto Doc::hardIndent(const Doc &other) const -> Doc
{
    // *this + (hardline() + other).nest(DEFAULT_INDENT)
    auto nested = Doc(makeNest(DEFAULT_INDENT, makeConcat(hardline().impl_, other.impl_)));
    return *this + nested;
}

auto Doc::hardDedent(const Doc &other) const -> Doc
{
    // *this + (hardline() + other).nest(-DEFAULT_INDENT)
    auto nested = Doc(makeNest(-DEFAULT_INDENT, makeConcat(hardline().impl_, other.impl_)));
    return *this + nested;
}

auto Doc::bracket(const Doc &left, const Doc &inner, const Doc &right) -> Doc
{
    return left << (inner >> right);
}

auto Doc::group() const -> Doc
{
    return Doc(makeUnion(flatten(impl_), impl_));
}

// Rendering
auto Doc::render(int width) const -> std::string
{
    Renderer renderer(width, 2); // Default 2 spaces per indent level
    return renderer.render(impl_);
}

auto Doc::render(int width, int indent_size) const -> std::string
{
    Renderer renderer(width, indent_size);
    return renderer.render(impl_);
}

auto Doc::render(const common::Config &config) const -> std::string
{
    const int width = static_cast<int>(config.line_config.line_length);
    const int indent_size = static_cast<int>(config.line_config.indent_size);
    Renderer renderer(width, indent_size);
    return renderer.render(impl_);
}

// Helper functions
auto space() -> Doc
{
    return Doc::text(" ");
}

} // namespace emit
