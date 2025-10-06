#include <filesystem>

namespace vhdl_fmt {

class ConfigReader final
{
  public:
    explicit ConfigReader(std::filesystem::path config_file_path) :
      config_file_path(std::move(config_file_path)) {};

  private:
    /// Reads the config file
    auto readConfigFile() -> void;

    std::filesystem::path config_file_path;
};

} // namespace vhdl_fmt
