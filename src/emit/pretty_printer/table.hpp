#ifndef EMIT_TABLE_HPP
#define EMIT_TABLE_HPP

#include "emit/pretty_printer/doc.hpp"

#include <vector>

namespace emit {

/// Create a table with aligned columns
/// Columns are left-aligned with a single space separator
/// @param rows Vector of rows, where each row is a vector of Doc cells
/// @return Doc representing the aligned table
auto makeTable(const std::vector<std::vector<Doc>> &rows) -> Doc;

} // namespace emit

#endif // EMIT_TABLE_HPP
