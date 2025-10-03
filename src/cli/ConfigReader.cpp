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
    CliArgs cliArgs;

    try {
        m_parser.parse_args(static_cast<int>(m_args.size()), m_args.data());

        // Handle version flag
        if (m_parser.get<bool>("--version")) {
            cliArgs.show_version = true;
            return cliArgs;
        }

        // Handle help flag (argparse handles this automatically, but we track it)
        if (m_parser.get<bool>("--help")) {
            cliArgs.show_help = true;
            return cliArgs;
        }

        // Handle config location
        if (m_parser.is_used("--location")) {
            cliArgs.config_location = m_parser.get<std::string>("--location");
        }

        // Handle input files
        if (m_parser.is_used("files")) {
            auto files = m_parser.get<std::vector<std::string>>("files");
            for (const auto &file : files) {
                cliArgs.input_files.emplace_back(file);
            }
        }

        if (!validateArguments(cliArgs)) {
            throw std::invalid_argument("Invalid command line arguments");
        }

    } catch (const std::exception &e) {
        std::cerr << "Error parsing arguments: " << e.what() << std::endl;
        std::cerr << m_parser << std::endl;
        throw;
    }

    return cliArgs;
}

auto ConfigReader::getVersion() -> std::string
{
    return "vhdl-fmt 1.0.0";
}

auto ConfigReader::setupArgumentParser() -> void
{
    m_parser.add_description("A VHDL code formatter that automatically formats VHDL source files");

    // Version flag
    m_parser.add_argument("-v", "--version")
      .help("Show version information")
      .default_value(false)
      .implicit_value(true);

    // Help flag (argparse handles this automatically, but we add it for consistency)
    m_parser.add_argument("-h", "--help")
      .help("Show this help message and exit")
      .default_value(false)
      .implicit_value(true);

    // Config location flag
    m_parser.add_argument("-l", "--location")
      .help("Specify configuration file location (e.g., -l=/src/config/vhdl-fmt.yaml)")
      .metavar("PATH");

    // Input files (positional arguments)
    m_parser.add_argument("files").help("VHDL files to format").remaining().metavar("FILES");
}

auto ConfigReader::validateArguments(const CliArgs &args) -> bool
{
    // If version or help is requested, other validation is not needed
    if (args.show_version || args.show_help) {
        return true;
    }

    // Validate config location if provided
    if (!args.config_location.empty()) {
        if (!std::filesystem::exists(args.config_location.parent_path())) {
            std::cerr
              << "Error: Configuration file directory does not exist: "
              << args.config_location.parent_path()
              << std::endl;
            return false;
        }
    }

    // Validate input files
    for (const auto &file : args.input_files) {
        if (!std::filesystem::exists(file)) {
            std::cerr << "Error: Input file does not exist: " << file << std::endl;
            return false;
        }

        if (!std::filesystem::is_regular_file(file)) {
            std::cerr << "Error: Input path is not a regular file: " << file << std::endl;
            return false;
        }
    }

    return true;
}

} // namespace vhdl_fmt
