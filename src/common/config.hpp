#ifndef COMMON_CONFIG_HPP
#define COMMON_CONFIG_HPP

#include <cstdint>
#include <stdexcept>
#include <string>

namespace common {

/// General configuration for line wrapping and indentation
struct LineConfig final
{
    std::uint8_t line_length;
    std::uint8_t indent_size;
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

namespace helper {

static constexpr std::uint8_t DEFAULT_LINE_LENGTH{ 100 };
static constexpr std::uint8_t DEFAULT_INDENT_SIZE{ 4 };

static constexpr std::uint8_t MIN_LINE_LENGTH{ 10 };
static constexpr std::uint8_t MAX_LINE_LENGTH{ 200 };

static constexpr std::uint8_t MIN_INDENT_SIZE{ 1 };
static constexpr std::uint8_t MAX_INDENT_SIZE{ 16 };

/// Validate line configuration
inline auto validateLineConfig(const struct LineConfig &cfg) -> void
{
    if (cfg.line_length < MIN_LINE_LENGTH || cfg.line_length > MAX_LINE_LENGTH) {
        const std::string msg = "Line length must be between " + std::to_string(MIN_LINE_LENGTH)
                              + " and " + std::to_string(MAX_LINE_LENGTH) + ".";
        throw std::invalid_argument(msg);
    }

    if (cfg.indent_size < MIN_INDENT_SIZE || cfg.indent_size > MAX_INDENT_SIZE) {
        const std::string msg = "Indent size must be between " + std::to_string(MIN_INDENT_SIZE)
                              + " and " + std::to_string(MAX_INDENT_SIZE) + ".";
        throw std::invalid_argument(msg);
    }
}

} // namespace helper

struct Config final
{
    // INFO: Public members since its a POD struct
    // ---
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    LineConfig line_config{ .line_length = config_details::DEFAULT_LINE_LENGTH,
                            .indent_size = config_details::DEFAULT_INDENT_SIZE };
    IndentationStyle indent_style{ IndentationStyle::SPACES };
    EndOfLine eol{ EndOfLine::AUTO };
    PortMapConfig port_map;
    DeclarationConfig declarations;
    CasingConfig casing;
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

} // namespace common

#endif
