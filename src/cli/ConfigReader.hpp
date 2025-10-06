#include "Config.hpp"

#include <expected>
#include <filesystem>

namespace vhdl_fmt {

/// Custom error type for config file reading
struct ConfigReadError final
{
    std::string message;
};

class ConfigReader final
{
  public:
    /// Constructs a ConfigReader instance
    explicit ConfigReader(std::filesystem::path config_file_path) :
      config_file_path(std::move(config_file_path)) {};

    /// Reads the config file
    auto readConfigFile() -> std::expected<Config, ConfigReadError>;

  private:
    std::filesystem::path config_file_path;
};

} // namespace vhdl_fmt
