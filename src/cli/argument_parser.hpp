#ifndef CLI_ARGUMENT_PARSER_HPP
#define CLI_ARGUMENT_PARSER_HPP

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <unordered_map>

namespace cli {

enum class Arguments : std::uint8_t
{
    WRITE,
    CHECK
};

class ArgumentParser final
{
  public:
    /// Constructs an ArgumentParser instance and parses the cli arguments
    explicit ArgumentParser(std::span<char *> args);

    /// Returns the path to the config file
    [[nodiscard]]
    auto getConfigPath() const noexcept -> const std::optional<std::filesystem::path> &;

    /// Returns a map of the cli flags and their values
    [[nodiscard]]
    auto getFlags() const noexcept -> const std::unordered_map<Arguments, bool> &;

  private:
    /// Parses and processes the cli arguments
    auto parseArguments(std::span<char *> args) -> void;

    std::optional<std::filesystem::path> config_file_path_;
    std::unordered_map<Arguments, bool> used_flags_ = {
        { Arguments::WRITE, false },
        { Arguments::CHECK, false }
    };
};

} // namespace cli

#endif /* CLI_ARGUMENT_PARSER_HPP */
