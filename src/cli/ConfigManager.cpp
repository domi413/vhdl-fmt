#include "ConfigManager.hpp"

#include "Config.hpp"

#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace vhdl_fmt {

using CaseStyleMemberPtr = CaseStyle CasingConfig::*;
using BoolMemberPtr = bool DeclarationsConfig::*;

static const std::unordered_map<std::string_view, CaseStyle> CASE_STYLE = {
    { "lower_case", CaseStyle::LOWER },
    { "UPPER_CASE", CaseStyle::UPPER },
};

static const std::unordered_map<std::string_view, EndOfLine> EOL_STYLE = {
    { "auto", EndOfLine::AUTO },
    { "crlf", EndOfLine::CRLF },
    { "lf",   EndOfLine::LF   },
};

static const std::unordered_map<std::string_view, IndentationStyle> INDENTATION_STYLE = {
    { "spaces", IndentationStyle::SPACES },
    { "tabs",   IndentationStyle::TABS   },
};

static const std::unordered_map<std::string_view, CaseStyleMemberPtr> CASING_ASSIGNMENTS = {
    { "keywords",    &CasingConfig::keywords    },
    { "constants",   &CasingConfig::constants   },
    { "identifiers", &CasingConfig::identifiers },
};

static const std::unordered_map<std::string_view, BoolMemberPtr> DECLARATION_ASSIGNMENTS = {
    { "align_colons",         &DeclarationsConfig::align_colons         },
    { "align_types",          &DeclarationsConfig::align_types          },
    { "align_initialization", &DeclarationsConfig::align_initialization },
};

auto parseStyle(const std::string_view style,
                const std::unordered_map<std::string_view, CaseStyle> &map) -> CaseStyle
{
    if (const auto it = map.find(style); it != map.end()) {
        return it->second;
    }

    throw std::invalid_argument("Invalid case style config: " + std::string(style));
}

auto parseStyle(const std::string_view style,
                const std::unordered_map<std::string_view, EndOfLine> &map) -> EndOfLine
{
    if (const auto it = map.find(style); it != map.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid eol config: " + std::string(style));
}

auto parseStyle(const std::string_view style,
                const std::unordered_map<std::string_view, IndentationStyle> &map)
  -> IndentationStyle
{
    if (const auto it = map.find(style); it != map.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid indentation config: " + std::string(style));
}

// =========================================================================
// ConfigManager Implementation
// =========================================================================

ConfigManager::ConfigManager(std::span<char *> args) :
  config_reader(std::make_unique<ConfigReader>(args)),
  config(Config::getDefault())
{
    processConfiguration();
}

ConfigManager::ConfigManager(std::unique_ptr<IConfigReader> config_reader) :
  config_reader(std::move(config_reader)),
  config(Config::getDefault())
{
    processConfiguration();
}

auto ConfigManager::getConfig() const -> const Config &
{
    return config;
}

auto ConfigManager::getCliArgs() const -> const CliArgs &
{
    return cli_args;
}

auto ConfigManager::processConfiguration() -> void
{
    cli_args = config_reader->readConfig();

    if (cli_args.show_version || cli_args.show_help) {
        return;
    }

    Config yaml_config = Config::getDefault();
    std::filesystem::path config_path = findConfigFile();

    if (!cli_args.config_location.empty()) {
        config_path = cli_args.config_location;
    }

    if (!config_path.empty() && std::filesystem::exists(config_path)) {
        try {
            yaml_config = readConfigFile(config_path);
        } catch (const std::exception &e) {
            std::cerr
              << "Warning: Failed to read configuration file '"
              << config_path
              << "': "
              << e.what()
              << '\n';
            std::cerr << "Using default configuration." << '\n';
            yaml_config = Config::getDefault();
        }
    }

    config = mergeConfigurations(yaml_config);

    if (!config.validate()) {
        throw std::invalid_argument("Invalid configuration");
    }
}

auto ConfigManager::readConfigFile(const std::filesystem::path &path) -> Config
{
    if (!isValidConfigFile(path)) {
        throw std::invalid_argument("Invalid configuration file extension");
    }

    Config config = Config::getDefault();

    try {
        YAML::Node yaml_node = YAML::LoadFile(path.string());

        if (yaml_node["line_length"]) {
            config.line_length = yaml_node["line_length"].as<std::size_t>();
        }

        if (yaml_node["indentation"]) {
            const auto &indent_node = yaml_node["indentation"];
            if (indent_node["style"]) {
                const auto style_str = indent_node["style"].as<std::string>();
                config.indentation.style = parseStyle(style_str, INDENTATION_STYLE_MAP);
            }
            if (indent_node["size"]) {
                config.indentation.size = indent_node["size"].as<std::size_t>();
            }
        }

        if (yaml_node["end_of_line"]) {
            const auto eol_str = yaml_node["end_of_line"].as<std::string>();
            config.end_of_line = parseStyle(eol_str, EOL_STYLE_MAP);
        }

        if (yaml_node["formatting"]) {
            const auto &format_node = yaml_node["formatting"];

            if (format_node["port_map"] && format_node["port_map"]["align_signals"]) {
                config.formatting.port_map.align_signals
                  = format_node["port_map"]["align_signals"].as<bool>();
            }

            if (format_node["declarations"]) {
                const auto &decl_node = format_node["declarations"];

                // Data-driven loading for bool flags using pointer-to-member
                for (const auto &[key_str, member_ptr] : DECLARATION_ASSIGNMENTS) {
                    if (decl_node[key_str.data()]) {
                        config.formatting.declarations.*member_ptr
                          = decl_node[key_str.data()].as<bool>();
                    }
                }
            }
        }

        if (yaml_node["casing"]) {
            const auto &casing_node = yaml_node["casing"];

            // Data-driven loading for CaseStyle enums using pointer-to-member
            for (const auto &[key_str, member_ptr] : CASING_ASSIGNMENTS) {
                if (casing_node[key_str.data()]) {
                    const auto style_str = casing_node[key_str.data()].as<std::string>();
                    const CaseStyle style_enum = parseStyle(style_str, CASE_STYLE_MAP);

                    config.casing.*member_ptr = style_enum;
                }
            }
        }

    } catch (const YAML::Exception &e) {
        throw std::runtime_error("YAML parsing error: " + std::string(e.what()));
    }

    return config;
}

auto ConfigManager::findConfigFile() -> std::filesystem::path
{
    const std::vector<std::string> config_file_name
      = { "vhdl-fmt.yaml", "vhdl-fmt.yml", ".vhdl-fmt.yaml", ".vhdl-fmt.yml" };

    std::filesystem::path cwd = std::filesystem::current_path();

    for (const auto &file : config_file_name) {
        std::filesystem::path path = cwd / file;
        if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) {
            return path;
        }
    }

    return {};
}

auto ConfigManager::mergeConfigurations(const Config &yamlConfig) -> Config
{
    return yamlConfig;
}

auto ConfigManager::isValidConfigFile(const std::filesystem::path &path) -> bool
{
    const std::string extension = path.extension().string();
    return extension == ".yaml" || extension == ".yml";
}

} // namespace vhdl_fmt
