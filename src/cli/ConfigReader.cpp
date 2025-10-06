#include "ConfigReader.hpp"

#include "Config.hpp"

#include <filesystem>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

namespace vhdl_fmt {

// TODO(domi): Check how this would look like with a switch-case / if-else
// - Switch / If is simpler and maybe less code
// - Although hash map is O(1), with this small amount of options its prolly more overhead
// + The map is easier to extent with additional options
// + Map looks cooler (and is maybe "more" modern C++?)
// ? Ask Felix for review and opinion

namespace {

using LineConfigMember = std::variant<std::uint16_t LineConfig::*, std::uint8_t LineConfig::*>;
using PortMapMember = bool DeclarationConfig::*;
using DeclrationMember = bool DeclarationConfig::*;
using CaseStyleMember = CaseStyle CasingConfig::*;

constexpr std::unordered_map<std::string_view, LineConfigMember> LINE_CONFIG_ASSIGNMENTS_MAP = {
    { "line_length", &LineConfig::line_length },
    { "indent_size", &LineConfig::indent_size },
};

constexpr std::unordered_map<std::string_view, IndentationStyle> INDENTATION_STYLE_MAP = {
    { "spaces", IndentationStyle::SPACES },
    { "tabs",   IndentationStyle::TABS   },
};

constexpr std::unordered_map<std::string_view, EndOfLine> EOL_STYLE_MAP_MAP_MAP = {
    { "auto", EndOfLine::AUTO },
    { "crlf", EndOfLine::CRLF },
    { "lf",   EndOfLine::LF   },
};

constexpr std::unordered_map<std::string_view, CaseStyle> CASE_STYLE_MAP = {
    { "lower_case", CaseStyle::LOWER },
    { "UPPER_CASE", CaseStyle::UPPER },
};

constexpr std::unordered_map<std::string_view, PortMapMember> PORT_MAP_ASSIGNMENTS_MAP = {
    { "align_signals", &PortMapConfig::align_signals },
};

constexpr std::unordered_map<std::string_view, DeclrationMember> DECLARATION_ASSIGNMENTS_MAP = {
    { "align_colons",         &DeclarationConfig::align_colons         },
    { "align_types",          &DeclarationConfig::align_types          },
    { "align_initialization", &DeclarationConfig::align_initialization },
};

constexpr std::unordered_map<std::string_view, CaseStyleMember> CASING_ASSIGNMENTS_MAP = {
    { "keywords",    &CasingConfig::keywords    },
    { "constants",   &CasingConfig::constants   },
    { "identifiers", &CasingConfig::identifiers },
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

/// Checks if a node node exists and is not null
constexpr auto IS_VALID = [](const YAML::Node &node) -> bool { return node && !node.IsNull(); };

} // namespace

auto ConfigReader::readConfigFile() -> void
{
    if (!std::filesystem::exists(this->config_file_path)) {
        // TODO(domi): Do we have to create a Config object here with default arguments?
        return;
    }

    const YAML::Node root_node = YAML::LoadFile(this->config_file_path);

    if (!root_node.IsMap()) {
        throw std::runtime_error(
          "Config file is not a valid map or could not be correctly loaded.");
    }

    Config config{};

    // --- LineConfig (line length) ---
    if (const auto node = root_node["line_length"]; IS_VALID(node)) {
        config.line_config.line_length
          = parseStyle(node.as<std::uint16_t>(), LINE_CONFIG_ASSIGNMENTS_MAP, "line length");
    }

    // --- IndentationStyle & LineConfig (indent size) ---
    if (const auto node = root_node["indentation"]; IS_VALID(node)) {
        if (const auto style_node = node["style"]; IS_VALID(style_node)) {
            config.indent_style
              = parseStyle(node.as<std::string_view>(), INDENTATION_STYLE_MAP, "indentation style");
        }
        if (const auto size_node = node["size"]; IS_VALID(node)) {
            config.indent_size = parseStyle(
              node.as<std::uint8_t>(), LINE_CONFIG_ASSIGNMENTS_MAP, "indentation size");
        }
    }

    // --- EndOfLine ---
    if (const auto node = root_node["end_of_line"]; IS_VALID(node)) {
        config.eol
          = parseStyle(node.as<std::string_view>, EOL_STYLE_MAP_MAP_MAP, "end of line style");
    }

    // --- PortMapConfig ---
    if (const auto formatting_node = root_node["formatting"]; IS_VALID(formatting_node)) {
        if (const auto port_map_node = formatting_node["port_map"]; IS_VALID(port_map_node)) {
            for (const auto &[key, member_ptr] : PORT_MAP_ASSIGNMENTS_MAP) {
                if (const auto val_node = port_map_node[std::string{ key }]; is_valid(val_node)) {
                    loaded_config.port_map.*member_ptr = val_node.as<bool>();
                }
            }
        }

        // --- DeclarationConfig ---
        if (const auto declarations_node = formatting_node["declarations"];
            IS_VALID(declarations_node)) {
            for (const auto &[key, member_ptr] : DECLARATION_ASSIGNMENTS_MAP) {
                if (const auto val_node = declarations_node[std::string{ key }];
                    is_valid(val_node)) {
                    config.declarations.*member_ptr = val_node.as<bool>();
                }
            }
        }

        // --- CasingConfig ---
        if (const auto casing_node = root_node["casing"]; is_valid(casing_node)) {
            for (const auto &[key, member_ptr] : CASING_ASSIGNMENTS_MAP) {
                if (const auto val_node = casing_node[std::string{ key }]; is_valid(val_node)) {
                    const auto style_str = val_node.as<std::string_view>();
                    loaded_config.casing.*member_ptr
                      = parseStyle(style_str, CASE_STYLE_MAP, "casing");
                }
            }
        }
    }
}

} // namespace vhdl_fmt
