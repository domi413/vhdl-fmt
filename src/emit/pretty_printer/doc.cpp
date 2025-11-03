#include "emit/pretty_printer/doc.hpp"

#include "emit/pretty_printer/doc_impl.hpp"
#include "emit/pretty_printer/renderer.hpp"

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

// Combinators
auto Doc::operator+(const Doc &other) const -> Doc
{
    return Doc(makeConcat(impl_, other.impl_));
}

auto Doc::nest(int indent) const -> Doc
{
    return Doc(makeNest(indent, impl_));
}

auto Doc::group() const -> Doc
{
    return Doc(makeUnion(impl_, impl_));
}

// Rendering
auto Doc::render(int width) const -> std::string
{
    Renderer renderer(width);
    return renderer.render(impl_);
}

void Doc::renderToStream(std::ostream &out, int width) const
{
    out << render(width);
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
