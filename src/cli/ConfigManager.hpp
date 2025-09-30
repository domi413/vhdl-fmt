namespace vhdl_fmt {

class ConfigManager
{
  public:
    /// Creates a new ConfigManager and parses the CLI arguments
    explicit ConfigManager(std::span<char *> args);

  private:
    /// Parses and processes the CLI arguments
    auto parseArguments(std::span<char *> args) -> void;

    /// Reads the configuration file
    auto readConfigFile(std::filesystem::path &path) -> void;
};

} // namespace vhdl_fmt
