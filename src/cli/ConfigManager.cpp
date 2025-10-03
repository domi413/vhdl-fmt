#include "ConfigManager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace vhdl_fmt {

ConfigManager::ConfigManager(std::span<char *> args) :
  m_configReader(std::make_unique<ConfigReader>(args)),
  m_config(Config::getDefault())
{
    processConfiguration();
}

ConfigManager::ConfigManager(std::unique_ptr<IConfigReader> configReader) :
  m_configReader(std::move(configReader)),
  m_config(Config::getDefault())
{
    processConfiguration();
}

auto ConfigManager::getConfig() const -> const Config &
{
    return m_config;
}

auto ConfigManager::getCliArgs() const -> const CliArgs &
{
    return m_cliArgs;
}

auto ConfigManager::processConfiguration() -> void
{
    // Parse CLI arguments
    m_cliArgs = m_configReader->readConfig();

    // If version or help is requested, no need to process configuration
    if (m_cliArgs.show_version || m_cliArgs.show_help) {
        return;
    }

    Config yamlConfig = Config::getDefault();

    // Determine config file path
    std::filesystem::path configPath;
    if (!m_cliArgs.config_location.empty()) {
        // Use explicitly provided config file
        configPath = m_cliArgs.config_location;
    } else {
        // Look for config file in current working directory
        configPath = findConfigFile();
    }

    // Read configuration file if it exists
    if (!configPath.empty() && std::filesystem::exists(configPath)) {
        try {
            yamlConfig = readConfigFile(configPath);
        } catch (const std::exception &e) {
            std::cerr
              << "Warning: Failed to read configuration file '"
              << configPath
              << "': "
              << e.what()
              << std::endl;
            std::cerr << "Using default configuration." << std::endl;
            yamlConfig = Config::getDefault();
        }
    }

    // Merge configurations (YAML serves as base, CLI can override)
    m_config = mergeConfigurations(yamlConfig);

    // Validate final configuration
    if (!m_config.validate()) {
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
        YAML::Node yamlNode = YAML::LoadFile(path.string());

        // Read line_length
        if (yamlNode["line_length"]) {
            config.line_length = yamlNode["line_length"].as<std::size_t>();
        }

        // Read indentation settings
        if (yamlNode["indentation"]) {
            const auto &indentNode = yamlNode["indentation"];

            if (indentNode["style"]) {
                std::string style = indentNode["style"].as<std::string>();
                if (style == "spaces") {
                    config.indentation.style = IndentationStyle::SPACES;
                } else if (style == "tabs") {
                    config.indentation.style = IndentationStyle::TABS;
                }
            }

            if (indentNode["size"]) {
                config.indentation.size = indentNode["size"].as<std::size_t>();
            }
        }

        // Read end_of_line
        if (yamlNode["end_of_line"]) {
            std::string eol = yamlNode["end_of_line"].as<std::string>();
            if (eol == "lf") {
                config.end_of_line = EndOfLine::LF;
            } else if (eol == "crlf") {
                config.end_of_line = EndOfLine::CRLF;
            } else if (eol == "auto") {
                config.end_of_line = EndOfLine::AUTO;
            }
        }

        // Read formatting settings
        if (yamlNode["formatting"]) {
            const auto &formatNode = yamlNode["formatting"];

            if (formatNode["port_map"] && formatNode["port_map"]["align_signals"]) {
                config.formatting.port_map.align_signals
                  = formatNode["port_map"]["align_signals"].as<bool>();
            }

            if (formatNode["declarations"]) {
                const auto &declNode = formatNode["declarations"];

                if (declNode["align_colons"]) {
                    config.formatting.declarations.align_colons
                      = declNode["align_colons"].as<bool>();
                }

                if (declNode["align_types"]) {
                    config.formatting.declarations.align_types = declNode["align_types"].as<bool>();
                }

                if (declNode["align_initialization"]) {
                    config.formatting.declarations.align_initialization
                      = declNode["align_initialization"].as<bool>();
                }
            }
        }

        // Read casing settings
        if (yamlNode["casing"]) {
            const auto &casingNode = yamlNode["casing"];

            auto parseCaseStyle = [](const std::string &style) -> CaseStyle {
                if (style == "lower")
                    return CaseStyle::LOWER;
                if (style == "upper")
                    return CaseStyle::UPPER;
                if (style == "snake_case")
                    return CaseStyle::SNAKE_CASE;
                return CaseStyle::LOWER; // default
            };

            if (casingNode["keywords"]) {
                config.casing.keywords = parseCaseStyle(casingNode["keywords"].as<std::string>());
            }

            if (casingNode["constants"]) {
                config.casing.constants = parseCaseStyle(casingNode["constants"].as<std::string>());
            }

            if (casingNode["identifiers"]) {
                config.casing.identifiers
                  = parseCaseStyle(casingNode["identifiers"].as<std::string>());
            }
        }

    } catch (const YAML::Exception &e) {
        throw std::runtime_error("YAML parsing error: " + std::string(e.what()));
    }

    return config;
}

auto ConfigManager::findConfigFile() -> std::filesystem::path
{
    const std::vector<std::string> configNames
      = { "vhdl-fmt.yaml", "vhdl-fmt.yml", ".vhdl-fmt.yaml", ".vhdl-fmt.yml" };

    std::filesystem::path cwd = std::filesystem::current_path();

    for (const auto &configName : configNames) {
        std::filesystem::path configPath = cwd / configName;
        if (std::filesystem::exists(configPath) && std::filesystem::is_regular_file(configPath)) {
            return configPath;
        }
    }

    return {}; // Return empty path if no config file found
}

auto ConfigManager::mergeConfigurations(const Config &yamlConfig) -> Config
{
    // For now, we just return the YAML config since CLI doesn't override YAML settings
    // In the future, CLI flags could override specific YAML settings
    return yamlConfig;
}

auto ConfigManager::isValidConfigFile(const std::filesystem::path &path) -> bool
{
    const std::string extension = path.extension().string();
    return extension == ".yaml" || extension == ".yml";
}

} // namespace vhdl_fmt
