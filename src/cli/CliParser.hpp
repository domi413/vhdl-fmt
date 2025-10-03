#ifndef VHDL_FMT_CLI_PARSER_HPP
#define VHDL_FMT_CLI_PARSER_HPP

#include "ConfigReader.hpp"

namespace vhdl_fmt {

/// Legacy alias for ConfigReader to maintain backward compatibility
/// @deprecated Use ConfigReader instead
using CliParser = ConfigReader;

} // namespace vhdl_fmt

#endif // VHDL_FMT_CLI_PARSER_HPP
