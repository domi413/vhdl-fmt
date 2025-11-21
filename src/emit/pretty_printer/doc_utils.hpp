#ifndef EMIT_DOC_UTILS_HPP
#define EMIT_DOC_UTILS_HPP

#include "emit/pretty_printer/doc.hpp"

#include <algorithm>
#include <ranges>

namespace emit {

/// @brief Maps items to Docs and joins them with a separator, avoiding intermediate vectors.
/// @param items The input range of items.
/// @param sep The separator Doc (e.g., Doc::line()).
/// @param transform Function to convert each item to a Doc.
/// @param with_trailing Whether to include the separator after the last element.
/// @return Combined Doc.
template<std::ranges::input_range Range, typename Transform>
auto joinMap(Range &&items, const Doc &sep, Transform &&transform, bool with_trailing) -> Doc
{
    return std::ranges::fold_left_first(
             std::forward<Range>(items) | std::views::transform(std::forward<Transform>(transform)),
             [&](const Doc &acc, const Doc &item) { return acc + sep + item; })
      .transform([&](const Doc &d) { return with_trailing ? d + sep : d; })
      .value_or(Doc::empty());
}

} // namespace emit

#endif // EMIT_DOC_UTILS_HPP
