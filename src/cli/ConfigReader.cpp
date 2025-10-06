#include "ConfigReader.hpp"

#include "Config.hpp"

#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace vhdl_fmt {

// TODO(domi): Check how this would look like with a switch-case / if-else
// - Switch / If is simpler and maybe less code
// - Although hash map is O(1), with this small amount of options its prolly more overhead
// + The map is easier to extent with additional options
// + Map looks cooler (and is maybe "more" modern C++?)
// ? Ask Felix for review and opinion

namespace {

constexpr std::unordered_map<std::string_view, CaseStyle> CASE_STYLE = {
    { "lower_case", CaseStyle::LOWER },
    { "UPPER_CASE", CaseStyle::UPPER },
};

constexpr std::unordered_map<std::string_view, EndOfLine> EOL_STYLE = {
    { "auto", EndOfLine::AUTO },
    { "crlf", EndOfLine::CRLF },
    { "lf",   EndOfLine::LF   },
};

constexpr std::unordered_map<std::string_view, IndentationStyle> INDENTATION_STYLE = {
    { "spaces", IndentationStyle::SPACES },
    { "tabs",   IndentationStyle::TABS   },
};

constexpr std::unordered_map<std::string_view, CasingConfig> CASING_ASSIGNMENTS = {
    { "keywords",    &CasingConfig::keywords    },
    { "constants",   &CasingConfig::constants   },
    { "identifiers", &CasingConfig::identifiers },
};

constexpr std::unordered_map<std::string_view, DeclarationConfig> DECLARATION_ASSIGNMENTS = {
    { "align_colons",         &DeclarationConfig::align_colons         },
    { "align_types",          &DeclarationConfig::align_types          },
    { "align_initialization", &DeclarationConfig::align_initialization },
};

template<typename T>
auto parseStyle(const std::string_view style,
                const std::unordered_map<std::string_view, T> &map,
                const std::string_view error_context) -> T
{
    if (const auto it = map.find(style); it != map.end()) {
        return it->second;
    }

    throw std::invalid_argument(std::string("Invalid ") + error_context.data()
                                + " config: " + style.data());
}

} // namespace

auto ConfigReader::readConfigFile() -> void
{
    YAML::Node config_file = YAML::Loadfile{ this->config_file_path };

    if (!config_file.IsMap()) {
        throw std::runtime_error(
          "Config file is not a valid map or could not be correctly loaded.");
    }

    for (const auto &option : options) {
        if (!config_file[option]) {
            throw std::runtime_error("Unknown " + option + " in config file.");
        }
    }
}

} // namespace vhdl_fmt

// line_length
// indentation
//   style
//   size
// end_of_line
// formatting
//   port_map
//     align_signals
//   declarations
//     align_colons
//     align_types
//     align_initialization
// casing
//   keywords
//   constants
//   identifiers
