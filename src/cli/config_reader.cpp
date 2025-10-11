#include "config_reader.hpp"

#include "common/config.hpp"

#include <array>
#include <cstdint>
#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>

namespace cli {

namespace {

using PortMapMemberPtr = bool common::PortMapConfig::*;
using DeclarationMemberPtr = bool common::DeclarationConfig::*;
using CasingMemberPtr = common::CaseStyle common::CasingConfig::*;

constexpr std::array INDENTATION_STYLE_MAP = {
    std::pair{ "spaces", common::IndentationStyle::SPACES },
    std::pair{ "tabs",   common::IndentationStyle::TABS   },
};

constexpr std::array EOL_STYLE_MAP = {
    std::pair{ "auto", common::EndOfLine::AUTO },
    std::pair{ "crlf", common::EndOfLine::CRLF },
    std::pair{ "lf",   common::EndOfLine::LF   },
};

constexpr std::array PORT_MAP_ASSIGNMENTS_MAP = {
    std::pair{ "align_signals", &common::PortMapConfig::align_signals },
};

constexpr std::array DECLARATION_ASSIGNMENTS_MAP = {
    std::pair{ "align_colons",         &common::DeclarationConfig::align_colons         },
    std::pair{ "align_types",          &common::DeclarationConfig::align_types          },
    std::pair{ "align_initialization", &common::DeclarationConfig::align_initialization },
};

constexpr std::array CASE_STYLE_MAP = {
    std::pair{ "lower_case", common::CaseStyle::LOWER },
    std::pair{ "UPPER_CASE", common::CaseStyle::UPPER },
};

constexpr std::array CASING_ASSIGNMENTS_MAP = {
    std::pair{ "keywords",    &common::CasingConfig::keywords    },
    std::pair{ "constants",   &common::CasingConfig::constants   },
    std::pair{ "identifiers", &common::CasingConfig::identifiers },
};

/// Helper for constexpr lookup in arrays
template<typename T, std::size_t N>
[[nodiscard]]
constexpr auto findInMap(const std::array<std::pair<std::string_view, T>, N> &map,
                         std::string_view key) -> std::optional<T>
{
    for (const auto &[k, v] : map) {
        if (k == key) {
            return v;
        }
    }
    return std::nullopt;
}

/// Checks if a node exists and is not null
[[nodiscard]]
inline auto isValid(const YAML::Node &node) -> bool
{
    return node && !node.IsNull();
}

template<typename T>
[[nodiscard]]
auto tryParseYaml(const YAML::Node &node, std::string_view name) -> std::optional<T>
{
    if (!isValid(node)) {
        return std::nullopt;
    }

    try {
        return node.as<T>();
    } catch (const YAML::BadConversion &e) {
        throw std::runtime_error(
          std::format("Invalid value for config field '{}': {}", name, e.what()));
    }
}

template<typename T, std::size_t N>
[[nodiscard]]
auto mapValueToConfig(std::string_view style,
                      const std::array<std::pair<std::string_view, T>, N> &map,
                      std::string_view error_context) -> T
{
    if (auto result = findInMap(map, style)) {
        return *result;
    }

    throw std::invalid_argument(std::format("Invalid {} config: {}", error_context, style));
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
          .message = "Config file does not exist at the defined location." } };
    }

    YAML::Node root_node{};
    try {
        root_node = YAML::LoadFile(path_to_read.string());
    } catch (const YAML::BadFile &e) {
        return std::unexpected{ ConfigReadError{
          .message = std::format("Could not load config file: {}", e.what()) } };
    } catch (const std::exception &e) {
        return std::unexpected{ ConfigReadError{
          .message = std::format("Error reading config file: {}", e.what()) } };
    }

    if (!root_node.IsNull() && !root_node.IsMap()) {
        return std::unexpected{ ConfigReadError{
          .message = "Config file is not a valid yaml file or could not be correctly loaded." } };
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
        return std::unexpected{ ConfigReadError{
          .message = std::format("Config parsing failed: {}", e.what()) } };
    }
}

auto ConfigReader::readLineconfig(const YAML::Node &root_node, const common::LineConfig &defaults)
  -> common::LineConfig
{
    auto line_config = defaults;

    if (const auto value = tryParseYaml<std::uint16_t>(root_node["line_length"], "line_length")) {
        line_config.line_length = *value;
    }

    if (const auto node = root_node["indentation"]; isValid(node)) {
        if (auto value = tryParseYaml<std::uint8_t>(node["size"], "indentation.size")) {
            line_config.indent_size = *value;
        }
    }

    const common::LineLength length_to_validate{ .length = line_config.line_length };
    const common::IndentSize size_to_validate{ .size = line_config.indent_size };
    common::LineConfig::validateLineConfig(length_to_validate, size_to_validate);

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
