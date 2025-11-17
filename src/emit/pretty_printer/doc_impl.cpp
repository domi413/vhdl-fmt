#include "emit/pretty_printer/doc_impl.hpp"

#include "common/overload.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

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

auto makeHardLines(unsigned count) -> DocPtr
{
    return std::make_shared<DocImpl>(HardLines{ count });
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

auto makeAlignText(std::string_view text, int level) -> DocPtr
{
    return std::make_shared<DocImpl>(AlignText{ .content = std::string(text), .level = level });
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

    return transformImpl(doc, [](const auto &node) {
        using T = std::decay_t<decltype(node)>;

        if constexpr (std::is_same_v<T, SoftLine>) {
            return makeText(" ");
        } else if constexpr (std::is_same_v<T, Union>) {
            return node.flat;
        } else if constexpr (std::is_same_v<T, AlignText>) {
            // In flat mode, an alignment text is just its text content.
            return makeText(node.content);
        } else if constexpr (std::is_same_v<T, Align>) {
            // In flat mode, an align group is just its inner content.
            return node.doc;
        } else {
            return std::make_shared<DocImpl>(node);
        }
    });
}

auto resolveAlignment(const DocPtr &doc) -> DocPtr
{
    // === Pass 1: Find max width FOR EACH level ===
    std::map<int, int> max_widths_by_level;
    max_widths_by_level = foldImpl(
      doc, std::move(max_widths_by_level), [](std::map<int, int> current_maxes, const auto &node) {
          using T = std::decay_t<decltype(node)>;
          if constexpr (std::is_same_v<T, AlignText>) {
              const int current_max = current_maxes[node.level];
              current_maxes[node.level]
                = std::max(current_max, static_cast<int>(node.content.length()));
          }
          return current_maxes; // Pass accumulator through
      });

    // Handle the case where no aligned text was found
    if (max_widths_by_level.empty()) {
        return doc;
    }

    // === Pass 2: Apply padding based on the level's max width ===
    return transformImpl(doc, [&](const auto &node) -> DocPtr {
        using T = std::decay_t<decltype(node)>;

        if constexpr (std::is_same_v<T, AlignText>) {
            // Look up the max width for this text's level
            const int max_width = max_widths_by_level.at(node.level);
            const int padding = max_width - static_cast<int>(node.content.length());
            return makeText(node.content + std::string(padding, ' '));
        } else {
            return std::make_shared<DocImpl>(node);
        }
    });
}

auto optimizeImpl(const DocPtr &doc) -> DocPtr
{
    auto optimization_rules = common::Overload{

        // === Concat nodes ===
        [](const Concat &node) -> DocPtr {
            // Rule 1: Identity (Empty) elimination
            const bool left_is_empty = std::holds_alternative<Empty>(node.left->value);
            const bool right_is_empty = std::holds_alternative<Empty>(node.right->value);

            if (left_is_empty && right_is_empty) {
                return makeEmpty(); // (empty + empty) -> empty
            }
            if (left_is_empty) {
                return node.right; // (empty + a) -> a
            }
            if (right_is_empty) {
                return node.left; // (a + empty) -> a
            }

            // Rule 2: Merge adjacent Text nodes
            if (auto *left_text = std::get_if<Text>(&node.left->value)) {
                if (auto *right_text = std::get_if<Text>(&node.right->value)) {
                    // (text("a") + text("b")) -> text("ab")
                    return makeText(left_text->content + right_text->content);
                }
            }

            // Rule 3: Merge adjacent HardLines/HardLine nodes
            auto get_lines = [](const DocPtr &d) -> unsigned {
                if (std::holds_alternative<HardLine>(d->value)) {
                    return 1;
                }
                if (auto *hl = std::get_if<HardLines>(&d->value)) {
                    return hl->count;
                }
                return 0; // Not a line node
            };

            const unsigned left_lines = get_lines(node.left);
            if (left_lines > 0) {
                const unsigned right_lines = get_lines(node.right);
                if (right_lines > 0) {
                    // (hardlines(A) + hardlines(B)) -> hardlines(A + B)
                    return makeHardLines(left_lines + right_lines);
                }
            }

            // If no Concat-specific rule matched, just return the node
            return std::make_shared<DocImpl>(node);
        },

        // === Default for all other nodes ===
        // (Text, HardLines, Empty, etc.)
        // These nodes have no optimization rules themselves, so we
        // just return them as-is.
        [](const auto &node) -> DocPtr { return std::make_shared<DocImpl>(node); }
    };

    // Apply the optimization rules recursively
    return transformImpl(doc, optimization_rules);
}

} // namespace emit
