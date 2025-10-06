#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace vhdl_fmt {

// TODO(domi): Fix {} config so its on the same line

class ConfigReader
{
  public:
    explicit ConfigReader(std::filesystem::path config_file_path) :
      config_file_path(std::move(config_file_path)) {};

  private:
    /// Reads the config file
    auto readConfigFile() -> void;

    std::filesystem::path config_file_path;
    Config config{};
};

} // namespace vhdl_fmt
