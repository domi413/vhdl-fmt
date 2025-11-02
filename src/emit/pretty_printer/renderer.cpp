#include "emit/pretty_printer/renderer.hpp"

namespace emit {

auto Renderer::render(const DocPtr &doc) -> std::string
{
    throw "not implemented";
}

// Internal rendering using visitor pattern with std::visit
void Renderer::renderDoc(int indent, Mode mode, const DocPtr &doc)
{
    throw "not implemented";
}

// Check if document fits on current line
auto Renderer::fits(int width, const DocPtr &doc) -> bool
{
    throw "not implemented";
}

// Output helpers
void Renderer::write(std::string_view text)
{
    throw "not implemented";
}

void Renderer::newline(int indent)
{
    throw "not implemented";
}

} // namespace emit
