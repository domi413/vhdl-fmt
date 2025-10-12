#ifndef CLI_CONFIG_READER_HPP
#define CLI_CONFIG_READER_HPP

#include "common/config.hpp"

#include <expected>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <yaml-cpp/node/convert.h>     // NOLINT(misc-include-cleaner)
#include <yaml-cpp/node/detail/impl.h> // NOLINT(misc-include-cleaner)
#include <yaml-cpp/node/node.h>

namespace cli {

/// Custom error type for config file reading
struct ConfigReadError final
{
    std::string message;
};

class ConfigReader final
{
  public:
    /// Constructs a ConfigReader instance
    explicit ConfigReader(std::optional<std::filesystem::path> config_file_path) :
      config_file_path(std::move(config_file_path))
    {
    }

    /// Reads the config file and returns either a Config object or a ConfigReadError
    [[nodiscard]]
    auto readConfigFile() -> std::expected<common::Config, ConfigReadError>;

  private:
    /// Reads the line config options
    [[nodiscard]]
    static auto readLineconfig(const YAML::Node &root_node, const common::LineConfig &defaults)
      -> common::LineConfig;

    /// Reads the indentation style options
    [[nodiscard]]
    static auto readIndentationStyle(const YAML::Node &root_node,
                                     const common::IndentationStyle &defaults)
      -> common::IndentationStyle;

    /// Reads the end of line options
    [[nodiscard]]
    static auto readEndOfLine(const YAML::Node &root_node, const common::EndOfLine &defaults)
      -> common::EndOfLine;

    /// Reads the port map options
    [[nodiscard]]
    static auto readPortMapConfig(const YAML::Node &root_node,
                                  const common::PortMapConfig &defaults) -> common::PortMapConfig;

    /// Reads the declaration options
    [[nodiscard]]
    static auto readDeclarationConfig(const YAML::Node &root_node,
                                      const common::DeclarationConfig &defaults)
      -> common::DeclarationConfig;

    /// Reads the casing options
    [[nodiscard]]
    static auto readCasingConfig(const YAML::Node &root_node, const common::CasingConfig &defaults)
      -> common::CasingConfig;

    std::optional<std::filesystem::path> config_file_path;
};

} // namespace cli

#endif /* CLI_CONFIG_READER_HPP */
