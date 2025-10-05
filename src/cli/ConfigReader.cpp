#include "ConfigReader.hpp"

#include "Config.hpp"

#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace vhdl_fmt {

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

} // namespace vhdl_fmt
