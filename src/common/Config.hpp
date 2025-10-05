#ifndef VHDL_FMT_CONFIG_HPP
#define VHDL_FMT_CONFIG_HPP

#include <cstdint>

namespace vhdl_fmt {

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

/// Casing conventions
enum class CaseStyle : std::uint8_t
{
    LOWER,
    UPPER,
};

/// General configuration for line wrapping and indentation
struct LineConfig final
{
    uint16_t line_length;
    uint8_t indent_size;
};

/// Port map signal alignment configuration
struct PortMapConfig final
{
    bool align_signals{ true };
};

/// Declaration alignment configuration
struct DeclarationsConfig final
{
    bool align_colons{ true };
    bool align_types{ true };
    bool align_initialization{ true };
};

/// Specific casing configuration that overwrites the default casing
struct CasingConfig final
{
    CaseStyle keywords = CaseStyle::LOWER;
    CaseStyle constants = CaseStyle::UPPER;
    CaseStyle identifiers = CaseStyle::LOWER;
};

/// Main configuration structure
struct Config final
{

  public:
    /// Creates a config object that holds the user config
    Config(IndentationStyle indent_style,
           LineConfig line_config,
           EndOfLine eol,
           PortMapConfig port_map,
           DeclarationsConfig declarations,
           CasingConfig casing) :
      indent_style(indent_style),
      eol(eol),
      line_config(line_config),
      port_map(port_map),
      declarations(declarations),
      casing(casing)
    {
        validate();
    }

  private:
    static constexpr uint8_t DEFAULT_LINE_LENGTH{ 100 };
    static constexpr uint8_t DEFAULT_INDENT_SIZE{ 4 };

    static constexpr uint8_t MIN_LINE_LENGTH{ 10 };
    static constexpr uint8_t MAX_LINE_LENGTH{ 200 };

    static constexpr uint8_t MIN_INDENT_SIZE{ 1 };
    static constexpr uint8_t MAX_INDENT_SIZE{ 16 };

    IndentationStyle indent_style{ IndentationStyle::SPACES };
    EndOfLine eol{ EndOfLine::AUTO };

    LineConfig line_config{ .line_length = DEFAULT_LINE_LENGTH,
                            .indent_size = DEFAULT_INDENT_SIZE };
    PortMapConfig port_map;
    DeclarationsConfig declarations;
    CasingConfig casing;

    /// Validates the configuration
    auto validate() const -> void;
};

} // namespace vhdl_fmt

#endif
