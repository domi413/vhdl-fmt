#ifndef VHDL_FMT_ARGUMENT_PARSER_HPP
#define VHDL_FMT_ARGUMENT_PARSER_HPP

#include <filesystem>
#include <span>

namespace vhdl_fmt {

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

    /// Displays the program version
    static auto showversion() -> void;

    std::filesystem::path config_file_path = std::filesystem::current_path();
};

} // namespace vhdl_fmt

#endif
