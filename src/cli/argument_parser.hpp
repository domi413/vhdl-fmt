#ifndef CLI_ARGUMENT_PARSER_HPP
#define CLI_ARGUMENT_PARSER_HPP

#include <bitset>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>

namespace cli {

enum class ArgumentFlag : std::uint8_t
{
    WRITE = 0,
    CHECK = 1,
    FLAG_COUNT = 2 // Required for flag count
};

class ArgumentParser final
{
  public:
    /// Constructs an ArgumentParser instance and parses the cli arguments
    explicit ArgumentParser(std::span<char *> args);

    /// Returns the path to the config file
    [[nodiscard]]
    auto getConfigPath() const noexcept -> const std::optional<std::filesystem::path> &;

    /// Returns whether a specific flag is set
    [[nodiscard]]
    auto isFlagSet(ArgumentFlag flag) const noexcept -> bool;

  private:
    /// Parses and processes the cli arguments
    auto parseArguments(std::span<char *> args) -> void;

    std::optional<std::filesystem::path> config_file_path_;
    std::bitset<static_cast<std::size_t>(ArgumentFlag::FLAG_COUNT)> used_flags_;
};

} // namespace cli

#endif /* CLI_ARGUMENT_PARSER_HPP */
