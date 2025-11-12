#include "emit/pretty_printer/doc_impl.hpp"

#include "emit/pretty_printer/doc.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace emit {

// Factory functions
auto makeEmpty() -> DocPtr
{
    return std::make_shared<DocImpl>(Empty{});
}

auto makeText(std::string_view text) -> DocPtr
{
    return std::make_shared<DocImpl>(Text{ std::string(text) });
}

auto makeLine() -> DocPtr
{
    return std::make_shared<DocImpl>(SoftLine{});
}

auto makeHardLine() -> DocPtr
{
    return std::make_shared<DocImpl>(HardLine{});
}

auto makeConcat(DocPtr left, DocPtr right) -> DocPtr
{
    return std::make_shared<DocImpl>(Concat{ .left = std::move(left), .right = std::move(right) });
}

auto makeNest(DocPtr doc) -> DocPtr
{
    return std::make_shared<DocImpl>(Nest{ .doc = std::move(doc) });
}

auto makeUnion(DocPtr flat, DocPtr broken) -> DocPtr
{
    return std::make_shared<DocImpl>(Union{ .flat = std::move(flat), .broken = std::move(broken) });
}

auto makeAlignPlaceholder(std::string_view text) -> DocPtr
{
    return std::make_shared<DocImpl>(AlignPlaceholder{ std::string(text) });
}

auto makeAlign(DocPtr doc) -> DocPtr
{
    return std::make_shared<DocImpl>(Align{ .doc = std::move(doc) });
}

// Utility functions
auto flatten(const DocPtr &doc) -> DocPtr
{
    if (!doc) {
        return doc;
    }

    return transformRecursive(doc, [](const auto &node) {
        using T = std::decay_t<decltype(node)>;

        if constexpr (std::is_same_v<T, SoftLine>) {
            return makeText(" ");
        } else if constexpr (std::is_same_v<T, Union>) {
            return node.flat;
        } else {
            return std::make_shared<DocImpl>(node);
        }
    });
}

auto resolveAlignment(const DocPtr &doc) -> DocPtr
{
    // === Pass 1: The "Fold" (using our new 'fold' method) ===
    int max_width = foldRecursive(doc, 0, [](int current_max, const auto &node) {
        using T = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<T, AlignPlaceholder>) {
            return std::max(current_max, static_cast<int>(node.content.length()));
        }
        return current_max; // Pass accumulator through
    });

    // Handle the case where no aligned text was found
    if (max_width == 0) {
        return doc;
    }

    // === Pass 2: The "Transform" (unchanged) ===
    return transformRecursive(doc, [&](const auto &node) -> DocPtr {
        using T = std::decay_t<decltype(node)>;

        if constexpr (std::is_same_v<T, AlignPlaceholder>) {
            const int padding = max_width - static_cast<int>(node.content.length());
            return makeText(node.content + std::string(padding, ' '));
        } else {
            return std::make_shared<DocImpl>(node);
        }
    });
}

} // namespace emit
