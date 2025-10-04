#ifndef VHDL_FMT_CONFIG_HPP
#define VHDL_FMT_CONFIG_HPP

#include <filesystem>
#include <vector>

namespace vhdl_fmt {

/// Enumeration for indentation style
enum class IndentationStyle
{
    SPACES,
    TABS
};

/// Enumeration for end of line style
enum class EndOfLine
{
    LF,
    CRLF,
    AUTO
};

/// Enumeration for case formatting
enum class CaseStyle
{
    LOWER,
    UPPER,
};

/// Configuration for port map formatting
struct PortMapConfig
{
    bool align_signals = true;
};

/// Configuration for declarations formatting
struct DeclarationsConfig
{
    bool align_colons = true;
    bool align_types = true;
    bool align_initialization = true;
};

/// Configuration for formatting options
struct FormattingConfig
{
    PortMapConfig port_map;
    DeclarationsConfig declarations;
};

/// Configuration for indentation
struct IndentationConfig
{
    IndentationStyle style = IndentationStyle::SPACES;
    std::size_t size = 4;
};

/// Configuration for case formatting
struct CasingConfig
{
    CaseStyle keywords = CaseStyle::LOWER;
    CaseStyle constants = CaseStyle::UPPER;
    CaseStyle identifiers = CaseStyle::LOWER;
};

/// Main configuration structure
struct Config
{
    std::size_t line_length = 100;
    IndentationConfig indentation;
    EndOfLine end_of_line = EndOfLine::AUTO;
    FormattingConfig formatting;
    CasingConfig casing;

    /// Returns default configuration
    static auto getDefault() -> Config;

    /// Validates the configuration
    [[nodiscard]] auto validate() const -> bool;
};

/// CLI arguments structure
struct CliArgs
{
    bool show_version = false;
    bool show_help = false;
    std::filesystem::path config_location;
    std::vector<std::filesystem::path> input_files;
};

} // namespace vhdl_fmt

#endif // VHDL_FMT_CONFIG_HPP
