#ifndef EMIT_DOC_UTILS_HPP
#define EMIT_DOC_UTILS_HPP

#include "emit/pretty_printer/doc.hpp"

#include <functional>
#include <ranges>
#include <vector>

namespace emit {

/// Convert a range of items into a vector of Docs using a transform function
/// @param items The input range of items
/// @param transform Function to convert each item to a Doc
/// @return Vector of Docs
template<std::ranges::input_range Range, typename Transform>
auto toDocVector(Range &&items, Transform &&transform) -> std::vector<Doc>
{
    std::vector<Doc> result{};

    if constexpr (std::ranges::sized_range<Range>) {
        result.reserve(std::ranges::size(items));
    }

    for (auto &&item : std::forward<Range>(items)) {
        result.emplace_back(std::invoke(std::forward<Transform>(transform), item));
    }

    return result;
}

/// Join multiple Docs with a separator
/// @param docs Vector of docs to join
/// @param separator Separator doc (e.g., Doc::text(";"))
/// @param with_trailing Whether to include separator after the last element
/// @return Combined Doc
inline auto joinDocs(const std::vector<Doc> &docs, const Doc &separator, bool with_trailing) -> Doc
{
    if (docs.empty()) {
        return Doc::empty();
    }

    auto result = docs.front();

    for (const auto &doc : docs | std::views::drop(1)) {
        result += separator + doc;
    }

    if (with_trailing) {
        result += separator;
    }

    return result;
}

/// @brief Joins a range of items, allowing a position-aware
///        transform to handle separators and trivia correctly.
/// @param items The input range of items (e.g., generics, ports)
/// @param transform Function: Doc(const auto& item, bool is_last)
/// @return Combined Doc
template<std::ranges::input_range Range, typename Transform>
auto joinDocsConditional(Range &items, Transform &&transform) -> Doc
{
    if (std::ranges::empty(items)) {
        return Doc::empty();
    }

    Doc result = Doc::empty();
    auto it = std::ranges::begin(items);
    auto end = std::ranges::end(items);

    while (it != end) {
        const bool is_last = (std::next(it) == end);
        result += std::invoke(std::forward<Transform>(transform), *it, is_last);
        ++it;
    }
    return result;
}

} // namespace emit

#endif // EMIT_DOC_UTILS_HPP
