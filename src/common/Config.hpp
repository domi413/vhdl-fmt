#ifndef VHDL_FMT_CONFIG_HPP
#define VHDL_FMT_CONFIG_HPP

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace vhdl_fmt {

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

/// Main configuration structure
struct Config final
{

  public:
    // Default constructor uses default member initializers
    Config() { validate(); }

    // Explicit constructor to fully initialize
    Config(IndentationStyle indent_style,
           EndOfLine eol,
           LineConfig line_config,
           PortMapConfig port_map,
           DeclarationConfig declarations,
           CasingConfig casing) :
      line_config(line_config),
      indent_style(indent_style),
      eol(eol),
      port_map(port_map),
      declarations(declarations),
      casing(casing)
    {
        validate();
    }

    /// Returns the line configuration
    [[nodiscard]] auto getLineConfig() const noexcept -> const LineConfig & { return line_config; }

    /// Returns the indentation style
    [[nodiscard]] auto getIndentStyle() const noexcept -> IndentationStyle { return indent_style; }

    /// Returns the end of line sequence
    [[nodiscard]] auto getEol() const noexcept -> EndOfLine { return eol; }

    /// Returns the port map configuration
    [[nodiscard]] auto getPortMapConfig() const noexcept -> const PortMapConfig &
    {
        return port_map;
    }

    /// Returns the declaration configuration
    [[nodiscard]] auto getDeclarationConfig() const noexcept -> const DeclarationConfig &
    {
        return declarations;
    }

    /// Returns the casing configuration
    [[nodiscard]] auto getCasingConfig() const noexcept -> const CasingConfig & { return casing; }

  private:
    static constexpr std::uint8_t DEFAULT_LINE_LENGTH{ 100 };
    static constexpr std::uint8_t DEFAULT_INDENT_SIZE{ 4 };

    static constexpr std::uint8_t MIN_LINE_LENGTH{ 10 };
    static constexpr std::uint8_t MAX_LINE_LENGTH{ 200 };

    static constexpr std::uint8_t MIN_INDENT_SIZE{ 1 };
    static constexpr std::uint8_t MAX_INDENT_SIZE{ 16 };

    /// Validates the configuration
    auto validate() const -> void
    {
        if (this->line_config.line_length < MIN_LINE_LENGTH
            || this->line_config.line_length > MAX_LINE_LENGTH) {
            const std::string msg = "Line length must be between " + std::to_string(MIN_LINE_LENGTH)
                                  + " and " + std::to_string(MAX_LINE_LENGTH) + ".";

            throw std::invalid_argument(msg);
        }

        if (this->line_config.indent_size < MIN_INDENT_SIZE
            || this->line_config.indent_size > MAX_INDENT_SIZE) {
            const std::string msg = "Indent size must be between " + std::to_string(MIN_INDENT_SIZE)
                                  + " and " + std::to_string(MAX_INDENT_SIZE) + ".";

            throw std::invalid_argument(msg);
        }
    }

    LineConfig line_config{ .line_length = DEFAULT_LINE_LENGTH,
                            .indent_size = DEFAULT_INDENT_SIZE };
    IndentationStyle indent_style{ IndentationStyle::SPACES };
    EndOfLine eol{ EndOfLine::AUTO };
    PortMapConfig port_map;
    DeclarationConfig declarations;
    CasingConfig casing;
};

} // namespace vhdl_fmt

#endif
