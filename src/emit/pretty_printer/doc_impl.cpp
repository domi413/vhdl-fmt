#include "emit/pretty_printer/doc_impl.hpp"

namespace emit {

// Factory functions
auto makeEmpty() -> DocPtr
{
    throw "not implemented";
}

auto makeText(std::string text) -> DocPtr
{
    throw "not implemented";
}

auto makeLine() -> DocPtr
{
    throw "not implemented";
}

auto makeConcat(DocPtr left, DocPtr right) -> DocPtr
{
    throw "not implemented";
}

auto makeNest(int indent, DocPtr doc) -> DocPtr
{
    throw "not implemented";
}

auto makeUnion(DocPtr flat, DocPtr broken) -> DocPtr
{
    throw "not implemented";
}

// Utility functions
auto flatten(const DocPtr &doc) -> DocPtr
{
    throw "not implemented";
}

} // namespace emit
