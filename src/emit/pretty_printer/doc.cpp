#include "emit/pretty_printer/doc.hpp"

#include "emit/pretty_printer/doc_impl.hpp"
#include "emit/pretty_printer/renderer.hpp"

namespace emit {

// Doc construction
Doc::Doc(std::shared_ptr<DocImpl> impl) : impl_(std::move(impl)) {}

Doc::Doc() : impl_(makeEmpty()) {}
Doc::~Doc() = default;

// Basic constructors
auto Doc::empty() -> Doc
{
    throw "not implemented";
}

auto Doc::text(std::string_view str) -> Doc
{
    throw "not implemented";
}

auto Doc::line() -> Doc
{
    throw "not implemented";
}

// Combinators
auto Doc::operator+(const Doc &other) const -> Doc
{
    throw "not implemented";
}

auto Doc::nest(int indent) const -> Doc
{
    throw "not implemented";
}

auto Doc::group() const -> Doc
{
    throw "not implemented";
}

// Rendering
auto Doc::render(int width) const -> std::string
{
    throw "not implemented";
}

void Doc::renderToStream(std::ostream &out, int width) const
{
    throw "not implemented";
}

// Internal access
auto Doc::impl() const -> const std::shared_ptr<DocImpl> &
{
    return impl_;
}

// Helper functions
auto space() -> Doc
{
    throw "not implemented";
}

} // namespace emit
