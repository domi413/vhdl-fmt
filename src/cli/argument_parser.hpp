#ifndef CLI_ARGUMENT_PARSER_HPP
#define CLI_ARGUMENT_PARSER_HPP

#include <filesystem>
#include <span>

namespace cli {

class ArgumentParser final
{
  public:
    /// Constructs an ArgumentParser instance and parses the cli arguments
    explicit ArgumentParser(std::span<char *> args);

    /// Returns the path to the config file
    [[nodiscard]] auto getConfigPath() const -> const std::filesystem::path &;

  private:
    /// Parses and processes the cli arguments
    auto parseArguments(std::span<char *> args) -> void;

    std::filesystem::path config_file_path = std::filesystem::current_path();
};

} // namespace cli

#endif /* CLI_ARGUMENT_PARSER_HPP */
