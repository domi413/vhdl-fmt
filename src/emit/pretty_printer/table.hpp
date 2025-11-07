#ifndef EMIT_TABLE_HPP
#define EMIT_TABLE_HPP

#include "emit/pretty_printer/doc.hpp"

#include <vector>

namespace common {
struct Config;
} // namespace common

namespace emit {

/// Create a table with aligned columns
/// Columns are left-aligned with a single space separator
/// @param rows Vector of rows, where each row is a vector of Doc cells
/// @param config Configuration used for rendering cells during width measurement
/// @return Doc representing the aligned table
auto makeTable(const std::vector<std::vector<Doc>> &rows, const common::Config &config) -> Doc;

} // namespace emit

#endif // EMIT_TABLE_HPP
