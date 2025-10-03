#include "Config.hpp"

namespace vhdl_fmt {

auto Config::getDefault() -> Config
{
    Config config;

    // Set default values (already set in struct definition, but being explicit)
    config.line_length = 100;

    config.indentation.style = IndentationStyle::SPACES;
    config.indentation.size = 4;

    config.end_of_line = EndOfLine::AUTO;

    config.formatting.port_map.align_signals = true;
    config.formatting.declarations.align_colons = true;
    config.formatting.declarations.align_types = true;
    config.formatting.declarations.align_initialization = true;

    config.casing.keywords = CaseStyle::LOWER;
    config.casing.constants = CaseStyle::UPPER;
    config.casing.identifiers = CaseStyle::SNAKE_CASE;

    return config;
}

auto Config::validate() const -> bool
{
    // Validate line length
    if (line_length < 1 || line_length > 1000) {
        return false;
    }

    // Validate indentation size
    if (indentation.size < 1 || indentation.size > 16) {
        return false;
    }

    // All enum values are inherently valid due to type safety
    return true;
}

} // namespace vhdl_fmt
