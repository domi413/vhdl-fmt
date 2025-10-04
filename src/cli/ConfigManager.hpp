#include "ConfigReader.hpp"

#include <filesystem>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

namespace vhdl_fmt {

ConfigReader::ConfigReader(std::span<char *> args) :
  m_parser("vhdl-fmt", "VHDL Code Formatter"),
  m_args(args)
{
    setupArgumentParser();
}

auto ConfigReader::readConfig() -> CliArgs
{
    CliArgs cli_args;

    try {
        m_parser.parse_args(static_cast<int>(m_args.size()), m_args.data());

        // Extract and assign simple flags directly
        cli_args.show_version = m_parser.get<bool>("--version");
        cli_args.show_help = m_parser.get<bool>("--help");

        // If a terminal flag is set, return early without further processing/validation
        if (cli_args.show_version || cli_args.show_help) {
            return cli_args;
        }

        // Handle config location (using optional return to simplify check)
        if (const auto &location_opt = m_parser.get<std::optional<std::string>>("--location")) {
            cli_args.config_location = *location_opt;
        }

        // Handle input files (using optional return to simplify check)
        if (const auto &files_opt
            = m_parser.get<std::optional<std::vector<std::string>>>("files")) {
            for (const auto &file : *files_opt) {
                cli_args.input_files.emplace_back(file);
            }
        }

        if (!validateArguments(cli_args)) {
            throw std::invalid_argument("Invalid command line arguments");
        }

    } catch (const std::exception &e) {
        std::cerr << "Error parsing arguments: " << e.what() << '\n';
        // Only print help if the error occurred before the help flag could be checked/parsed
        if (!cli_args.show_help) {
            std::cerr << m_parser << '\n';
        }
        throw;
    }

    return cli_args;
}

auto ConfigReader::getVersion() -> std::string
{
    return "vhdl-fmt 1.0.0";
}

auto ConfigReader::setupArgumentParser() -> void
{
    m_parser.add_description("A VHDL code formatter that automatically formats VHDL source files");

    m_parser.add_argument("-v", "--version")
      .help("Show version information")
      .default_value(false)
      .implicit_value(true);

    m_parser.add_argument("-h", "--help")
      .help("Show this help message and exit")
      .default_value(false)
      .implicit_value(true);

    // Note: argparse::ArgumentParser should be configured to return std::optional<T> for optional
    // args.
    m_parser.add_argument("-l", "--location")
      .help("Specify configuration file location (e.g., -l=/src/config/vhdl-fmt.yaml)")
      .metavar("PATH")
      .nargs(argparse::nargs_pattern::optional); // Ensure this is treated as optional/nullable

    m_parser.add_argument("files")
      .help("VHDL files to format")
      .remaining()
      .nargs(argparse::nargs_pattern::optional) // Ensure this is treated as optional/nullable
      .metavar("FILES");
}

auto ConfigReader::validateArguments(const CliArgs &args) -> bool
{
    // Validation only proceeds if show_version/show_help are false (checked in readConfig)

    // Validate config location if provided
    if (!args.config_location.empty()) {
        if (!std::filesystem::exists(args.config_location.parent_path())) {
            std::cerr
              << "Error: Configuration file directory does not exist: "
              << args.config_location.parent_path()
              << '\n';
            return false;
        }
    }

    // Validate input files
    for (const auto &file : args.input_files) {
        if (!std::filesystem::exists(file)) {
            std::cerr << "Error: Input file does not exist: " << file << '\n';
            return false;
        }

        if (!std::filesystem::is_regular_file(file)) {
            std::cerr << "Error: Input path is not a regular file: " << file << '\n';
            return false;
        }
    }

    return true;
}

} // namespace vhdl_fmt
