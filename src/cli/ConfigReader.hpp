#include <filesystem>

namespace vhdl_fmt {

// TODO(domi): Fix {} config so its on the same line

class ConfigReader
{
  public:
    explicit ConfigReader(std::filesystem::path config_file_path) :
      config_file_path(std::move(config_file_path)) {};

  private:
    std::filesystem::path config_file_path;
};

} // namespace vhdl_fmt
