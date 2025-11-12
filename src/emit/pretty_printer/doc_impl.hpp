#ifndef EMIT_DOC_IMPL_HPP
#define EMIT_DOC_IMPL_HPP

#include "common/overload.hpp"

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

struct Align
{
    DocPtr doc;

    template<typename Fn>
    auto fmap(Fn &&fn) const -> Align
    {
        return { std::forward<Fn>(fn)(doc) };
    }
};

/// Internal document representation using variant
class DocImpl
{
  public:
    // NOLINTNEXTLINE (misc-non-private-member-variables-in-classes)
    std::variant<Empty, Text, SoftLine, HardLine, Concat, Nest, Union, AlignPlaceholder, Align>
      value;

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

    // 1. Define the visitor using Overload.
    // The visitor takes the accumulator T and the node, and returns the new accumulator T.
    auto recursive_folder = common::Overload{
        // ------------------------------------
        // Base Cases (Non-Recursive)
        // These nodes have no children, so we just apply 'fn' and return the result.
        // ------------------------------------
        [&fn](T current_value, const Empty &node) {
            return std::forward<Fn>(fn)(std::move(current_value), node);
        },
        [&fn](T current_value, const Text &node) {
            return std::forward<Fn>(fn)(std::move(current_value), node);
        },
        [&fn](T current_value, const SoftLine &node) {
            return std::forward<Fn>(fn)(std::move(current_value), node);
        },
        [&fn](T current_value, const HardLine &node) {
            return std::forward<Fn>(fn)(std::move(current_value), node);
        },
        [&fn](T current_value, const AlignPlaceholder &node) {
            return std::forward<Fn>(fn)(std::move(current_value), node);
        },

        // ------------------------------------
        // Recursive Cases (These perform recursive calls after applying 'fn')
        // ------------------------------------
        [&](T current_value, const Concat &node) {
            T new_value = std::forward<Fn>(fn)(std::move(current_value), node);
            new_value = foldRecursive(node.left, std::move(new_value), fn);
            new_value = foldRecursive(node.right, std::move(new_value), fn);
            return new_value;
        },
        [&](T current_value, const Nest &node) {
            T new_value = std::forward<Fn>(fn)(std::move(current_value), node);
            return foldRecursive(node.doc, std::move(new_value), fn);
        },
        [&](T current_value, const Union &node) {
            // Fold both branches for analysis purposes
            T new_value = std::forward<Fn>(fn)(std::move(current_value), node);
            new_value = foldRecursive(node.flat, std::move(new_value), fn);
            new_value = foldRecursive(node.broken, std::move(new_value), fn);
            return new_value;
        },
        [&](T current_value, const Align &node) {
            // Recurse into the aligned sub-document
            T new_value = std::forward<Fn>(fn)(std::move(current_value), node);
            return foldRecursive(node.doc, std::move(new_value), fn);
        }
    };

    // 2. Call std::visit with the explicit visitor.
    // If DocImpl::value ever gets a new node type, the compiler will error here
    // because recursive_folder won't have a matching overload.
    return std::visit(
      [&](const auto &node) {
          // Pass the initial accumulator (init) and the node to the visitor
          return recursive_folder(std::move(init), node);
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
auto makeAlign(DocPtr doc) -> DocPtr;

// Utility functions
auto flatten(const DocPtr &doc) -> DocPtr;
auto resolveAlignment(const DocPtr &doc) -> DocPtr;

} // namespace emit

#endif // EMIT_DOC_IMPL_HPP
