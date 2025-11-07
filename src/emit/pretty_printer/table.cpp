#include "emit/pretty_printer/table.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace emit {

// Use a very wide width to avoid line breaking when measuring cells
constexpr int CELL_RENDER_WIDTH = 10000;

auto makeTable(const std::vector<std::vector<Doc>> &rows) -> Doc
{
    if (rows.empty()) {
        return Doc::empty();
    }

    // Determine number of columns from first row
    const size_t num_columns = rows[0].size();
    if (num_columns == 0) {
        return Doc::empty();
    }

    // Step 1: Render all cells to measure widths
    std::vector<std::vector<std::string>> rendered_cells;
    std::vector<int> max_widths(num_columns, 0);

    for (const auto &row : rows) {
        std::vector<std::string> rendered_row;
        for (size_t col = 0; col < row.size() && col < num_columns; ++col) {
            // Render cell with a very wide width to avoid breaking
            std::string cell_text = row[col].render(CELL_RENDER_WIDTH);

            // Track max width for this column
            max_widths[col] = std::max(max_widths[col], static_cast<int>(cell_text.length()));

            rendered_row.push_back(std::move(cell_text));
        }
        rendered_cells.push_back(std::move(rendered_row));
    }

    // Step 2: Build aligned table as Doc
    Doc result = Doc::empty();
    bool first_row = true;

    for (const auto &rendered_row : rendered_cells) {
        if (!first_row) {
            result = result | Doc::empty(); // Add hardline between rows
        }
        first_row = false;

        Doc row_doc = Doc::empty();
        bool first_col = true;

        for (size_t col = 0; col < rendered_row.size(); ++col) {
            if (!first_col) {
                row_doc = row_doc + Doc::text(" "); // Single space separator
            }
            first_col = false;

            const std::string &cell_text = rendered_row[col];
            const int cell_width = static_cast<int>(cell_text.length());
            const int right_padding = max_widths[col] - cell_width;

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
