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
    std::vector<Doc> result;

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
/// @param include_trailing Whether to include separator after the last element
/// @return Combined Doc
inline auto joinDocs(const std::vector<Doc> &docs, const Doc &separator, bool include_trailing)
  -> Doc
{
    if (docs.empty()) {
        return Doc::empty();
    }

    auto result = docs.front();

    for (const auto &doc : docs | std::views::drop(1)) {
        result = result + separator + doc;
    }

    if (include_trailing) {
        result += separator;
    }

    return result;
}

} // namespace emit

#endif // EMIT_DOC_UTILS_HPP
