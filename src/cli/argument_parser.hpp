#ifndef CLI_ARGUMENT_PARSER_HPP
#define CLI_ARGUMENT_PARSER_HPP

#include <filesystem>
#include <span>
#include <string_view>
#include <unordered_map>

namespace cli {

class ArgumentParser final
{
  public:
    /// Constructs an ArgumentParser instance and parses the cli arguments
    explicit ArgumentParser(std::span<char *> args);

    /// Returns the path to the config file
    [[nodiscard]]
    auto getConfigPath() const noexcept -> const std::filesystem::path &;

    /// Returns a map of cli boolean flags and their values (either set or default)
    [[nodiscard]]
    auto getFlags() const noexcept -> const std::unordered_map<std::string_view, bool> &;

  private:
    /// Parses and processes the cli arguments
    auto parseArguments(std::span<char *> args) -> void;

    std::filesystem::path config_file_path_ = std::filesystem::current_path();
    std::unordered_map<std::string_view, bool> used_flags_ = {
        { "write", false },
        { "check", false }
    };
};

} // namespace cli

#endif /* CLI_ARGUMENT_PARSER_HPP */
