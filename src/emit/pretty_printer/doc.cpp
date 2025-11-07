#include "emit/pretty_printer/doc.hpp"

#include "emit/pretty_printer/doc_impl.hpp"
#include "emit/pretty_printer/renderer.hpp"

#include <memory>
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

auto Doc::group() const -> Doc
{
    return Doc(makeUnion(flatten(impl_), impl_));
}

// Rendering
auto Doc::render(int width) const -> std::string
{
    Renderer renderer(width);
    return renderer.render(impl_);
}

// Internal access
auto Doc::impl() const -> const std::shared_ptr<DocImpl> &
{
    return impl_;
}

// Helper functions
auto space() -> Doc
{
    return Doc::text(" ");
}

} // namespace emit
