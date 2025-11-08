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
        result.push_back(std::invoke(std::forward<Transform>(transform), item));
    }

    return result;
}

/// Convert a range of items into a vector of Docs by visiting each with a visitor
/// Convenience overload for visitor pattern
/// @param items The input range of items
/// @param visitor The visitor that can handle the items (must have a visit method or operator())
/// @return Vector of Docs
template<std::ranges::input_range Range, typename Visitor>
auto toDocVectorVisit(Range &&items, Visitor &visitor) -> std::vector<Doc>
{
    return toDocVector(std::forward<Range>(items),
                      [&visitor](auto &&item) { return visitor.visit(item); });
}

/// Create a row of Docs from a variadic list of arguments
/// Each argument is converted to text or kept as-is if already a Doc
/// @param args Variable number of string-like or Doc arguments
/// @return Vector of Docs representing a row
template<typename... Args>
auto makeRow(Args &&...args) -> std::vector<Doc>
{
    std::vector<Doc> row;
    row.reserve(sizeof...(args));

    (row.push_back([](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::same_as<T, Doc>) {
            return std::forward<decltype(arg)>(arg);
        } else {
            return Doc::text(std::forward<decltype(arg)>(arg));
        }
    }(std::forward<Args>(args))), ...);

    return row;
}

/// Join multiple Docs with a separator
/// @param docs Vector of docs to join
/// @param separator Separator doc (e.g., Doc::text(";"))
/// @param include_trailing Whether to include separator after the last element
/// @return Combined Doc
inline auto joinDocs(const std::vector<Doc> &docs,
                     const Doc &separator,
                     bool include_trailing) -> Doc
{
    if (docs.empty()) {
        return Doc::empty();
    }

    Doc result = Doc::empty();
    bool first = true;

    for (const auto &doc : docs) {
        if (!first) {
            result = result + separator;
        }
        result = result + doc;
        first = false;
    }

    if (include_trailing && !docs.empty()) {
        result = result + separator;
    }

    return result;
}

} // namespace emit

#endif // EMIT_DOC_UTILS_HPP
