#ifndef EMIT_DOC_IMPL_HPP
#define EMIT_DOC_IMPL_HPP

#include <memory>
#include <string>
#include <variant>

namespace emit {

// Forward declaration for recursive type
struct DocImpl;
using DocPtr = std::shared_ptr<DocImpl>;

/// Empty document
struct Empty
{};

/// Text (no newlines allowed)
struct Text
{
    std::string content;
};

/// Line break (space when flattened, newline when broken)
struct Line
{};

/// Concatenation of two documents
struct Concat
{
    DocPtr left;
    DocPtr right;
};

/// Increase indentation level
struct Nest
{
    int indent;
    DocPtr doc;
};

/// Choice between flat and broken layout
struct Union
{
    DocPtr flat;
    DocPtr broken;
};

/// Internal document representation using variant
class DocImpl
{
    using Variant = std::variant<Empty, Text, Line, Concat, Nest, Union>;
    Variant value_;

  public:
    // Convenience constructors
    explicit DocImpl(Empty e) : value_(e) {}
    explicit DocImpl(Text t) : value_(std::move(t)) {}
    explicit DocImpl(Line l) : value_(l) {}
    explicit DocImpl(Concat c) : value_(std::move(c)) {}
    explicit DocImpl(Nest n) : value_(std::move(n)) {}
    explicit DocImpl(Union u) : value_(std::move(u)) {}
};

// Factory functions for creating documents
auto makeEmpty() -> DocPtr;
auto makeText(std::string text) -> DocPtr;
auto makeLine() -> DocPtr;
auto makeConcat(DocPtr left, DocPtr right) -> DocPtr;
auto makeNest(int indent, DocPtr doc) -> DocPtr;
auto makeUnion(DocPtr flat, DocPtr broken) -> DocPtr;

// Utility functions
auto flatten(const DocPtr &doc) -> DocPtr;

} // namespace emit

#endif // EMIT_DOC_IMPL_HPP
