#ifndef VHDL_FMT_CONFIG_READER_HPP
#define VHDL_FMT_CONFIG_READER_HPP

#include "Config.hpp"

#include <argparse/argparse.hpp>
#include <span>
#include <string>

namespace vhdl_fmt {

/// Interface for reading configuration from various sources
class IConfigReader
{
  public:
    virtual ~IConfigReader() = default;
    virtual auto readConfig() -> CliArgs = 0;
};

/// Handles CLI argument parsing using argparse
class ConfigReader : public IConfigReader
{
  public:
    /// Constructor that takes command line arguments
    explicit ConfigReader(std::span<char *> args);

    /// Parses CLI arguments and returns the configuration
    auto readConfig() -> CliArgs override;

    /// Returns the version string
    static auto getVersion() -> std::string;

  private:
    argparse::ArgumentParser m_parser;
    std::span<char *> m_args;

    /// Sets up argument parser with all required options
    auto setupArgumentParser() -> void;

    /// Validates the parsed arguments
    auto validateArguments(const CliArgs &args) -> bool;
};

} // namespace vhdl_fmt

#endif // VHDL_FMT_CONFIG_READER_HPP
