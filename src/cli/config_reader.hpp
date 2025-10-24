#ifndef CLI_CONFIG_READER_HPP
#define CLI_CONFIG_READER_HPP

#include "common/config.hpp"

#include <expected>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>
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
    explicit ConfigReader(std::optional<std::filesystem::path> config_file_path) :
      config_file_path_(std::move(config_file_path))
    {
    }

    [[nodiscard]]
    auto readConfigFile() -> std::expected<common::Config, ConfigReadError>;

  private:
    [[nodiscard]]
    static auto readLineconfig(const YAML::Node &root_node, const common::LineConfig &defaults)
      -> common::LineConfig;

    [[nodiscard]]
    static auto readIndentationStyle(const YAML::Node &root_node,
                                     const common::IndentationStyle &defaults)
      -> common::IndentationStyle;

    [[nodiscard]]
    static auto readEndOfLine(const YAML::Node &root_node, const common::EndOfLine &defaults)
      -> common::EndOfLine;

    [[nodiscard]]
    static auto readPortMapConfig(const YAML::Node &root_node,
                                  const common::PortMapConfig &defaults) -> common::PortMapConfig;

    [[nodiscard]]
    static auto readDeclarationConfig(const YAML::Node &root_node,
                                      const common::DeclarationConfig &defaults)
      -> common::DeclarationConfig;

    [[nodiscard]]
    static auto readCasingConfig(const YAML::Node &root_node, const common::CasingConfig &defaults)
      -> common::CasingConfig;

    std::optional<std::filesystem::path> config_file_path_;
};

} // namespace cli

#endif /* CLI_CONFIG_READER_HPP */
