#ifndef EMIT_DOC_IMPL_HPP
#define EMIT_DOC_IMPL_HPP

#include <memory>
#include <string>
#include <string_view>
#include <utility>
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
struct LineBreak
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
  public:
    // NOLINTNEXTLINE (misc-non-private-member-variables-in-classes)
    std::variant<Empty, Text, LineBreak, Concat, Nest, Union> value;

    // Convenience constructors
    explicit DocImpl(Empty e) : value(e) {}
    explicit DocImpl(Text t) : value(std::move(t)) {}
    explicit DocImpl(LineBreak l) : value(l) {}
    explicit DocImpl(Concat c) : value(std::move(c)) {}
    explicit DocImpl(Nest n) : value(std::move(n)) {}
    explicit DocImpl(Union u) : value(std::move(u)) {}
};

// Factory functions for creating documents
auto makeEmpty() -> DocPtr;
auto makeText(std::string_view text) -> DocPtr;
auto makeLine() -> DocPtr;
auto makeConcat(DocPtr left, DocPtr right) -> DocPtr;
auto makeNest(int indent, DocPtr doc) -> DocPtr;
auto makeUnion(DocPtr flat, DocPtr broken) -> DocPtr;

// Utility functions
auto flatten(const DocPtr &doc) -> DocPtr;

} // namespace emit

#endif // EMIT_DOC_IMPL_HPP
