#include "emit/pretty_printer/table.hpp"

#include "common/config.hpp"
#include "emit/pretty_printer/doc.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <string>
#include <vector>

namespace emit {

// Use a very wide width to avoid line breaking when measuring cells
constexpr auto CONFIG = common::Config{ .line_config = { .line_length = 10000 } };

auto makeTable(const std::vector<std::vector<Doc>> &rows) -> Doc
{
    if (rows.empty() || rows[0].empty()) {
        return Doc::empty();
    }

    const size_t num_columns = rows[0].size();

    // Step 1: Render all cells to measure widths
    std::vector<std::vector<std::string>> rendered_cells;
    std::vector<int> max_widths(num_columns, 0);

    for (const auto &row : rows) {
        auto &rendered_row = rendered_cells.emplace_back();
        for (auto [col, cell] : std::views::enumerate(row) | std::views::take(num_columns)) {
            auto &cell_text = rendered_row.emplace_back(cell.render(CONFIG));
            max_widths[col] = std::max(max_widths[col], static_cast<int>(cell_text.length()));
        }
    }

    // Step 2: Build aligned table as Doc
    Doc result = Doc::empty();

    for (auto [row_idx, rendered_row] : std::views::enumerate(rendered_cells)) {
        if (row_idx > 0) {
            result = result | Doc::empty(); // Add hardline between rows
        }

        Doc row_doc = Doc::empty();

        for (auto [col, cell_text] : std::views::enumerate(rendered_row)) {
            if (col > 0) {
                row_doc = row_doc + Doc::text(" "); // Single space separator
            }

            const int right_padding = max_widths[col] - static_cast<int>(cell_text.length());

            // Build cell with padding (left-aligned)
            Doc cell_doc = Doc::text(cell_text);
            // Add right padding to all columns except the last
            if (right_padding > 0 && col < rendered_row.size() - 1) {
                cell_doc = cell_doc + Doc::text(std::string(right_padding, ' '));
            }

            row_doc = row_doc + cell_doc;
        }

        result = result + row_doc;
    }

    return result;
}

} // namespace emit
