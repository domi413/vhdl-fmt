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

namespace {

using LineConfigMember = std::uint8_t LineConfig::*;
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

constexpr std::unordered_map<std::string_view, EndOfLine> EOL_STYLE_MAP = {
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

/// Checks if a node node exists and is not null
constexpr auto IS_VALID = [](const YAML::Node &node) -> bool { return node && !node.IsNull(); };

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

template<typename T>
auto parseScalar(const YAML::Node &node, const char *name) -> T
{
    if (!IS_VALID(node)) {
        throw std::runtime_error(std::string("Missing required config field: ") + name);
    }

    try {
        return node.as<T>();
    } catch (const YAML::BadConversion &e) {
        throw std::runtime_error(std::string("Invalid value for config field '") + name
                                 + "': " + e.what());
    }
}

} // namespace

auto ConfigReader::readConfigFile() -> std::expected<Config, ConfigReadError>
{
    if (!std::filesystem::exists(this->config_file_path)) {
        return Config{};
    }

    YAML::Node root_node{};
    try {
        root_node = YAML::LoadFile(this->config_file_path.string()); // Assignment works now
    } catch (const YAML::BadFile &e) {
        return std::unexpected{ ConfigReadError{ std::string("Could not load config file: ")
                                                 + e.what() } };
    } catch (const std::exception &e) {
        return std::unexpected{ ConfigReadError{ std::string("Error reading config file: ")
                                                 + e.what() } };
    }

    if (!root_node.IsMap()) {
        throw std::runtime_error(
          "Config file is not a valid map or could not be correctly loaded.");
    }

    const Config default_config{};

    IndentationStyle indent_style = default_config.getIndentStyle();
    EndOfLine eol = default_config.getEol();
    LineConfig line_config = default_config.getLineConfig();
    PortMapConfig port_map = default_config.getPortMapConfig();
    DeclarationConfig declarations = default_config.getDeclarationConfig();
    CasingConfig casing = default_config.getCasingConfig();

    try {
        // --- LineConfig (line length) ---
        if (const auto node = root_node["line_length"]; IS_VALID(node)) {
            line_config.line_length = parseScalar<std::uint8_t>(node, "line_length");
        }

        // --- IndentationStyle & LineConfig (indent size) ---
        if (const auto node = root_node["indentation"]; IS_VALID(node)) {
            if (const auto style_node = node["style"]; IS_VALID(style_node)) {
                const auto style_str
                  = parseScalar<std::string_view>(style_node, "indentation.style");
                indent_style = parseStyle(style_str, INDENTATION_STYLE_MAP, "indentation style");
            }
            if (const auto size_node = node["size"]; IS_VALID(size_node)) {
                line_config.indent_size = parseScalar<std::uint8_t>(size_node, "indentation.size");
            }
        }

        // --- EndOfLine ---
        if (const auto node = root_node["end_of_line"]; IS_VALID(node)) {
            const auto style_str = parseScalar<std::string_view>(node, "end_of_line");
            eol = parseStyle(style_str, EOL_STYLE_MAP, "end of line style");
        }

        // --- Formatting Sub-nodes (PortMapConfig, DeclarationConfig, CasingConfig) ---
        if (const auto formatting_node = root_node["formatting"]; IS_VALID(formatting_node)) {

            // --- PortMapConfig ---
            if (const auto port_map_node = formatting_node["port_map"]; IS_VALID(port_map_node)) {
                for (const auto &[key, member_ptr] : PORT_MAP_ASSIGNMENTS_MAP) {
                    if (const auto val_node = port_map_node[std::string{ key }];
                        IS_VALID(val_node)) {
                        port_map.*member_ptr = parseScalar<bool>(val_node, key.data());
                    }
                }
            }

            // --- DeclarationConfig ---
            if (const auto declarations_node = formatting_node["declarations"];
                IS_VALID(declarations_node)) {
                for (const auto &[key, member_ptr] : DECLARATION_ASSIGNMENTS_MAP) {
                    if (const auto val_node = declarations_node[std::string{ key }];
                        IS_VALID(val_node)) {
                        declarations.*member_ptr = parseScalar<bool>(val_node, key.data());
                    }
                }
            }

            // --- CasingConfig ---
            if (const auto casing_node = formatting_node["casing"]; IS_VALID(casing_node)) {
                for (const auto &[key, member_ptr] : CASING_ASSIGNMENTS_MAP) {
                    if (const auto val_node = casing_node[std::string{ key }]; IS_VALID(val_node)) {
                        const auto style_str = parseScalar<std::string_view>(val_node, key.data());
                        casing.*member_ptr = parseStyle(style_str, CASE_STYLE_MAP, "casing");
                    }
                }
            }
        }

        return Config{ indent_style, eol, line_config, port_map, declarations, casing };

    } catch (const std::exception &e) {
        return std::unexpected{ ConfigReadError{ std::string("Config parsing failed: ")
                                                 + e.what() } };
    }
}

} // namespace vhdl_fmt
