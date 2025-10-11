#include "config_reader.hpp"

#include "common/config.hpp"

#include <cstdint>
#include <exception>
#include <expected>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/parse.h>

namespace cli {

namespace {

// The map isn't `constexpr` as clang-tidy suggests. A singleton pattern would
// suppress the warning but reduce readability without significant benefit.
// ---
// NOLINTBEGIN(fuchsia-statically-constructed-objects)
// NOLINTBEGIN(cert-err58-cpp)

using PortMapMemberPtr = bool common::PortMapConfig::*;
using DeclarationMemberPtr = bool common::DeclarationConfig::*;
using CasingMemberPtr = common::CaseStyle common::CasingConfig::*;

const std::unordered_map<std::string_view, common::IndentationStyle> INDENTATION_STYLE_MAP = {
    { "spaces", common::IndentationStyle::SPACES },
    { "tabs",   common::IndentationStyle::TABS   },
};

const std::unordered_map<std::string_view, common::EndOfLine> EOL_STYLE_MAP = {
    { "auto", common::EndOfLine::AUTO },
    { "crlf", common::EndOfLine::CRLF },
    { "lf",   common::EndOfLine::LF   },
};

const std::unordered_map<std::string_view, PortMapMemberPtr> PORT_MAP_ASSIGNMENTS_MAP = {
    { "align_signals", &common::PortMapConfig::align_signals },
};

const std::unordered_map<std::string_view, DeclarationMemberPtr> DECLARATION_ASSIGNMENTS_MAP = {
    { "align_colons",         &common::DeclarationConfig::align_colons         },
    { "align_types",          &common::DeclarationConfig::align_types          },
    { "align_initialization", &common::DeclarationConfig::align_initialization },
};

const std::unordered_map<std::string_view, common::CaseStyle> CASE_STYLE_MAP = {
    { "lower_case", common::CaseStyle::LOWER },
    { "UPPER_CASE", common::CaseStyle::UPPER },
};

const std::unordered_map<std::string_view, CasingMemberPtr> CASING_ASSIGNMENTS_MAP = {
    { "keywords",    &common::CasingConfig::keywords    },
    { "constants",   &common::CasingConfig::constants   },
    { "identifiers", &common::CasingConfig::identifiers },
};

// NOLINTEND(cert-err58-cpp)
// NOLINTEND(fuchsia-statically-constructed-objects)

/// Checks if a node exists and is not null
constexpr auto isValid(const YAML::Node &node) -> bool
{
    return node && !node.IsNull();
};

template<typename T>
auto tryParseYaml(const YAML::Node &node, std::string_view name) -> std::optional<T>
{
    if (!isValid(node)) {
        return std::nullopt;
    }

    try {
        return node.as<T>();
    } catch (const YAML::BadConversion &e) {
        throw std::runtime_error(std::string("Invalid value for config field '") + std::string(name)
                                 + "': " + e.what());
    }
}

template<typename T>
auto mapValueToConfig(const std::string_view style,
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

auto ConfigReader::readConfigFile() -> std::expected<common::Config, ConfigReadError>
{
    if (!config_file_path_.has_value()) {
        const auto default_path = std::filesystem::current_path() / "vhdl-fmt.yaml";

        if (!std::filesystem::exists(default_path)) {
            // When no config file location was passed, return the default config
            return common::Config{};
        }

        config_file_path_ = default_path;
    }

    const auto &path_to_read = config_file_path_.value();

    if (!std::filesystem::exists(path_to_read)) {
        return std::unexpected{ ConfigReadError{
          "Config file does not exist at the defined location." } };
    }

    YAML::Node root_node{};
    try {
        root_node = YAML::LoadFile(path_to_read.string());
    } catch (const YAML::BadFile &e) {
        return std::unexpected{ ConfigReadError{ std::string("Could not load config file: ")
                                                 + e.what() } };
    } catch (const std::exception &e) {
        return std::unexpected{ ConfigReadError{ std::string("Error reading config file: ")
                                                 + e.what() } };
    }

    if (!root_node.IsNull() && !root_node.IsMap()) {
        return std::unexpected{ ConfigReadError{
          "Config file is not a valid yaml file or could not be correctly loaded." } };
    }

    try {
        common::Config config{};

        config.casing = readCasingConfig(root_node, config.casing);
        config.port_map = readPortMapConfig(root_node, config.port_map);
        config.eol_format = readEndOfLine(root_node, config.eol_format);
        config.line_config = readLineconfig(root_node, config.line_config);
        config.indent_style = readIndentationStyle(root_node, config.indent_style);
        config.declarations = readDeclarationConfig(root_node, config.declarations);

        return config;
    } catch (const std::exception &e) {
        return std::unexpected{ ConfigReadError{ std::string("Config parsing failed: ")
                                                 + e.what() } };
    }
}

auto ConfigReader::readLineconfig(const YAML::Node &root_node, const common::LineConfig &defaults)
  -> common::LineConfig
{
    auto line_config = defaults;

    if (const auto value = tryParseYaml<std::uint8_t>(root_node["line_length"], "line_length")) {
        line_config.line_length = *value;
    }

    if (const auto node = root_node["indentation"]; isValid(node)) {
        if (auto value = tryParseYaml<std::uint8_t>(node["size"], "indentation.size")) {
            line_config.indent_size = *value;
        }
    }

    return line_config;
}

auto ConfigReader::readIndentationStyle(const YAML::Node &root_node,
                                        const common::IndentationStyle &defaults)
  -> common::IndentationStyle
{
    auto indent_style = defaults;

    if (const auto node = root_node["indentation"]; isValid(node)) {
        if (auto style_str = tryParseYaml<std::string_view>(node["style"], "indentation.style")) {
            indent_style = mapValueToConfig(*style_str, INDENTATION_STYLE_MAP, "indentation style");
        }
    }

    return indent_style;
}

auto ConfigReader::readEndOfLine(const YAML::Node &root_node, const common::EndOfLine &defaults)
  -> common::EndOfLine
{
    auto eol = defaults;

    if (const auto style_str
        = tryParseYaml<std::string_view>(root_node["end_of_line"], "end_of_line")) {
        eol = mapValueToConfig(*style_str, EOL_STYLE_MAP, "end of line style");
    }

    return eol;
}

auto ConfigReader::readPortMapConfig(const YAML::Node &root_node,
                                     const common::PortMapConfig &defaults) -> common::PortMapConfig
{
    auto port_map = defaults;

    if (const auto formatting_node = root_node["formatting"]; isValid(formatting_node)) {
        if (const auto port_map_node = formatting_node["port_map"]; isValid(port_map_node)) {
            for (const auto &[key, member_ptr] : PORT_MAP_ASSIGNMENTS_MAP) {
                if (auto value = tryParseYaml<bool>(port_map_node[std::string{ key }], key)) {
                    port_map.*member_ptr = *value;
                }
            }
        }
    }

    return port_map;
}

auto ConfigReader::readDeclarationConfig(const YAML::Node &root_node,
                                         const common::DeclarationConfig &defaults)
  -> common::DeclarationConfig
{
    auto declarations = defaults;

    if (const auto formatting_node = root_node["formatting"]; isValid(formatting_node)) {
        if (const auto declarations_node = formatting_node["declarations"];
            isValid(declarations_node)) {
            for (const auto &[key, member_ptr] : DECLARATION_ASSIGNMENTS_MAP) {
                if (auto value = tryParseYaml<bool>(declarations_node[std::string{ key }], key)) {
                    declarations.*member_ptr = *value;
                }
            }
        }
    }

    return declarations;
}

auto ConfigReader::readCasingConfig(const YAML::Node &root_node,
                                    const common::CasingConfig &defaults) -> common::CasingConfig
{
    auto casing = defaults;

    if (const auto formatting_node = root_node["formatting"]; isValid(formatting_node)) {
        if (const auto casing_node = formatting_node["casing"]; isValid(casing_node)) {
            for (const auto &[key, member_ptr] : CASING_ASSIGNMENTS_MAP) {
                if (auto style_str
                    = tryParseYaml<std::string_view>(casing_node[std::string{ key }], key)) {
                    casing.*member_ptr = mapValueToConfig(*style_str, CASE_STYLE_MAP, "casing");
                }
            }
        }
    }

    return casing;
}

} // namespace cli
