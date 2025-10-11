#ifndef COMMON_CONFIG_HPP
#define COMMON_CONFIG_HPP

#include <cstdint>
#include <format>
#include <stdexcept>
#include <string>

namespace common {

/// Line length
struct LineLength final
{
    std::uint8_t length;
};

/// Indentation size
struct IndentSize final
{
    std::uint8_t size;
};

/// Indentation style used for formatting
enum class IndentationStyle : std::uint8_t
{
    SPACES,
    TABS
};

/// End of line character sequence configuration
enum class EndOfLine : std::uint8_t
{
    LF,
    CRLF,
    AUTO
};

/// Port map signal alignment configuration
struct PortMapConfig final
{
    bool align_signals{ true };
};

/// Declaration alignment configuration
struct DeclarationConfig final
{
    bool align_colons{ true };
    bool align_types{ true };
    bool align_initialization{ true };
};

/// Casing conventions
enum class CaseStyle : std::uint8_t
{
    LOWER,
    UPPER,
};

/// Specific casing configuration that overwrites the default casing
struct CasingConfig final
{
    CaseStyle keywords{ CaseStyle::LOWER };
    CaseStyle constants{ CaseStyle::UPPER };
    CaseStyle identifiers{ CaseStyle::LOWER };
};

/// General configuration for line wrapping and indentation
struct LineConfig final
{
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    std::uint8_t line_length;
    std::uint8_t indent_size;
    // NOLINTEND(misc-non-private-member-variables-in-classes)

    static auto create(const LineLength length, const IndentSize size) -> LineConfig
    {
        validateLineConfig(length, size);
        return { length, size };
    }

    static auto defaultConfig() -> LineConfig
    {
        static constexpr LineLength DEFAULT_LENGTH{ .length = DEFAULT_LINE_LENGTH };
        static constexpr IndentSize DEFAULT_SIZE{ .size = DEFAULT_INDENT_SIZE };

        return create(DEFAULT_LENGTH, DEFAULT_SIZE);
    }

  private:
    static constexpr std::uint8_t DEFAULT_LINE_LENGTH{ 100 };
    static constexpr std::uint8_t DEFAULT_INDENT_SIZE{ 4 };

    static constexpr std::uint8_t MIN_LINE_LENGTH{ 10 };
    static constexpr std::uint8_t MAX_LINE_LENGTH{ 200 };

    static constexpr std::uint8_t MIN_INDENT_SIZE{ 1 };
    static constexpr std::uint8_t MAX_INDENT_SIZE{ 16 };

    LineConfig(const LineLength length, const IndentSize size) :
      line_length(length.length),
      indent_size(size.size)
    {
    }

    // TODO(domi): Mozilla style formats the constructor braces ugly

    /// Validate line configuration
    static auto validateLineConfig(const LineLength length, const IndentSize size) -> void
    {
        if (length.length < MIN_LINE_LENGTH || length.length > MAX_LINE_LENGTH) {
            const std::string msg = std::format(
              "Line length must be between {} and {}", MIN_LINE_LENGTH, MAX_LINE_LENGTH);
            throw std::invalid_argument(msg);
        }

        if (size.size < MIN_INDENT_SIZE || size.size > MAX_INDENT_SIZE) {
            const std::string msg = std::format(
              "Indent size must be between {} and {}", MIN_INDENT_SIZE, MAX_INDENT_SIZE);
            throw std::invalid_argument(msg);
        }
    }
};

// Holds the configuration from the config file
struct Config final
{
    LineConfig line_config{ LineConfig::defaultConfig() };
    IndentationStyle indent_style{ IndentationStyle::SPACES };
    EndOfLine eol_format{ EndOfLine::LF };
    PortMapConfig port_map{ true };
    DeclarationConfig declarations{ .align_colons = true };
    CasingConfig casing{ .keywords = CaseStyle::LOWER,
                         .constants = CaseStyle::UPPER,
                         .identifiers = CaseStyle::LOWER };
};

} // namespace common

#endif /* COMMON_CONFIG_HPP */
