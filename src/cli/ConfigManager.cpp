#ifndef VHDL_FMT_CONFIG_MANAGER_H
#define VHDL_FMT_CONFIG_MANAGER_H

#include <argparse/argparse.hpp>
#include <filesystem>
#include <span>
#include <yaml-cpp/yaml.h>

namespace vhdl_fmt {

class ConfigManager
{
  public:
    /// Creates a new ConfigManager and parses the CLI arguments
    explicit ConfigManager(std::span<char *> args);

  private:
    auto parseArguments(std::span<char *> args) -> void;
    auto readConfigFile(std::filesystem::path &path) -> void;
};

} // namespace vhdl_fmt

#endif
