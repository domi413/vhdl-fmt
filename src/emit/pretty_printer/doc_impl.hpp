#ifndef EMIT_DOC_IMPL_HPP
#define EMIT_DOC_IMPL_HPP

#include <memory>
#include <string>
#include <string_view>
#include <variant>
namespace emit {

// Forward declaration for recursive type
struct DocImpl;
using DocPtr = std::shared_ptr<DocImpl>;

template<typename T>
concept MappableNode = requires(const T &node) {
    // Test if T::fmap is callable with a function that takes const DocPtr&
    // and returns DocPtr.
    { node.fmap(std::declval<DocPtr(const DocPtr &)>()) } -> std::same_as<T>;
};

/// Empty document
struct Empty
{
    template<typename Fn>
    auto fmap(Fn && /* fn */) const -> Empty
    {
        return {};
    }
};

/// Text (no newlines allowed)
struct Text
{
    std::string content;

    template<typename Fn>
    auto fmap(Fn && /* fn */) const -> Text
    {
        return { content };
    }
};

/// Line break (space when flattened, newline when broken)
struct SoftLine
{
    template<typename Fn>
    auto fmap(Fn && /* fn */) const -> SoftLine
    {
        return {};
    }
};

/// Hard line break (always newline, never becomes space)
struct HardLine
{
    template<typename Fn>
    auto fmap(Fn && /* fn */) const -> HardLine
    {
        return {};
    }
};

/// Concatenation of two documents
struct Concat
{
    DocPtr left;
    DocPtr right;

    template<typename Fn>
    auto fmap(Fn &&fn) const -> Concat
    {
        return { std::forward<Fn>(fn)(left), std::forward<Fn>(fn)(right) };
    }
};

/// Increase indentation level
struct Nest
{
    DocPtr doc;

    template<typename Fn>
    auto fmap(Fn &&fn) const -> Nest
    {
        return { std::forward<Fn>(fn)(doc) };
    }
};

/// Choice between flat and broken layout
struct Union
{
    DocPtr flat;
    DocPtr broken;

    template<typename Fn>
    auto fmap(Fn &&fn) const -> Union
    {
        return { std::forward<Fn>(fn)(flat), std::forward<Fn>(fn)(broken) };
    }
};

struct AlignPlaceholder
{
    std::string content;

    template<typename Fn>
    auto fmap(Fn && /* fn */) const -> AlignPlaceholder
    {
        return { content };
    }
};

/// Internal document representation using variant
class DocImpl
{
  public:
    // NOLINTNEXTLINE (misc-non-private-member-variables-in-classes)
    std::variant<Empty, Text, SoftLine, HardLine, Concat, Nest, Union, AlignPlaceholder> value;

    // Since all members are public, this class is considered an aggregate type and allows for
    // aggregate initialization.
};

/// Recursive document transformer using fmap
template<typename Fn>
auto transformRecursive(const DocPtr &doc, Fn &&fn) -> DocPtr
{
    return std::visit(
      [&fn](const MappableNode auto &node) -> DocPtr {
          auto mapped
            = node.fmap([&fn](const DocPtr &inner) { return transformRecursive(inner, fn); });
          return std::forward<Fn>(fn)(mapped);
      },
      doc->value);
}

template<typename T, typename Fn>
auto foldRecursive(const DocPtr &doc, T init, Fn &&fn) -> T
{
    if (!doc) {
        return init;
    }

    // std::visit handles the variant in doc->value
    return std::visit(
      [&](const auto &node) {
          // 1. Fold the current node
          T new_value = std::forward<Fn>(fn)(std::move(init), node);

          // 2. Recurse into children and thread the accumulator
          using NodeType = std::decay_t<decltype(node)>;

          if constexpr (std::is_same_v<NodeType, Concat>) {
              new_value = foldRecursive(node.left, std::move(new_value), fn);
              new_value = foldRecursive(node.right, std::move(new_value), fn);
          } else if constexpr (std::is_same_v<NodeType, Nest>) {
              new_value = foldRecursive(node.doc, std::move(new_value), fn);
          } else if constexpr (std::is_same_v<NodeType, Union>) {
              // Fold both branches, as alignment might be in either
              new_value = foldRecursive(node.flat, std::move(new_value), fn);
              new_value = foldRecursive(node.broken, std::move(new_value), fn);
          }
          // Base cases (Empty, Text, etc.) have no children and just return
          return new_value;
      },
      doc->value);
}

// Factory functions for creating documents
auto makeEmpty() -> DocPtr;
auto makeText(std::string_view text) -> DocPtr;
auto makeLine() -> DocPtr;
auto makeHardLine() -> DocPtr;
auto makeConcat(DocPtr left, DocPtr right) -> DocPtr;
auto makeNest(DocPtr doc) -> DocPtr;
auto makeUnion(DocPtr flat, DocPtr broken) -> DocPtr;
auto makeAlignPlaceholder(std::string_view text) -> DocPtr;

// Utility functions
auto flatten(const DocPtr &doc) -> DocPtr;

} // namespace emit

#endif // EMIT_DOC_IMPL_HPP
