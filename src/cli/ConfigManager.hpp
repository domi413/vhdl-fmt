#ifndef VHDL_FMT_CONFIG_MANAGER_HPP
#define VHDL_FMT_CONFIG_MANAGER_HPP

#include "Config.hpp"
#include "ConfigReader.hpp"

#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <vector>

namespace vhdl_fmt {

/// Interface for managing configuration from multiple sources
class IConfigManager
{
  public:
    virtual ~IConfigManager() = default;
    virtual auto getConfig() const -> const Config & = 0;
    virtual auto getCliArgs() const -> const CliArgs & = 0;
};

/// Manages configuration by combining CLI arguments and YAML file settings
class ConfigManager : public IConfigManager
{
  public:
    /// Creates a new ConfigManager and processes the CLI arguments
    explicit ConfigManager(std::span<char *> args);

    /// Creates a ConfigManager with a custom config reader (for testing)
    explicit ConfigManager(std::unique_ptr<IConfigReader> configReader);

    /// Returns the final merged configuration
    auto getConfig() const -> const Config & override;

    /// Returns the CLI arguments
    auto getCliArgs() const -> const CliArgs & override;

  private:
    std::unique_ptr<IConfigReader> m_configReader;
    CliArgs m_cliArgs;
    Config m_config;

    /// Processes the CLI arguments and loads configuration
    auto processConfiguration() -> void;

    /// Reads the configuration file from the specified path
    auto readConfigFile(const std::filesystem::path &path) -> Config;

    /// Finds configuration file in current working directory
    auto findConfigFile() -> std::filesystem::path;

    /// Merges CLI arguments with YAML configuration
    auto mergeConfigurations(const Config &yamlConfig) -> Config;

    /// Validates configuration file extensions
    auto isValidConfigFile(const std::filesystem::path &path) -> bool;
};

} // namespace vhdl_fmt

#endif // VHDL_FMT_CONFIG_MANAGER_HPP
