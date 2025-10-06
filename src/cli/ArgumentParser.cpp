#include "ArgumentParser.hpp"

#include "version.hpp"

#include <argparse/argparse.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace vhdl_fmt {

ArgumentParser::ArgumentParser(std::span<char *> args)
{
    parseArguments(args);
}

auto ArgumentParser::getConfigPath() const -> const std::filesystem::path &
{
    return config_file_path;
}

auto ArgumentParser::parseArguments(std::span<char *> args) -> void
{
    // TODO(domi): After the `.in` file gets correctly generated, use PROJECT_NAME & PROJECT_VERSION
    argparse::ArgumentParser program{ "vhdl-fmt", "1.0.0" };

    program.add_description("A VHDL formatter tool for beautifying and standardizing VHDL code.");
    program.add_epilog("For more information, visit the project documentation.");

    program.add_argument("-w", "--write")
      .help("Overwrites the input file with formatted content")
      .default_value(false)
      .implicit_value(true);

    program.add_argument("-c", "--check")
      .help("Checks if the file is formatted correctly without modifying it")
      .default_value(false)
      .implicit_value(true);

    program.add_argument("-l", "--location")
      .help("Path to the configuration file (e.g., /path/to/vhdl-fmt.yaml)")
      .action([this](const std::string &value) -> std::string {
          const std::filesystem::path config_path{ value };

          if (!std::filesystem::exists(config_path)) {
              throw std::runtime_error("Configuration file does not exist: " + value);
          }

          if (!std::filesystem::is_regular_file(config_path)) {
              throw std::runtime_error("Configuration path is not a regular file: " + value);
          }

          config_file_path = std::filesystem::canonical(config_path);
          return value;
      });

    try {
        std::vector<std::string> arguments;
        arguments.reserve(args.size());

        // NOTE: This loop is required because `parse_args()` expects a c-style array.
        for (const auto *arg : args) {
            if (arg != nullptr) {
                arguments.emplace_back(arg);
            }
        }

        program.parse_args(arguments);

    } catch (const std::exception &err) {
        std::cerr << "Error parsing arguments: " << err.what() << '\n';
        std::cerr << program << '\n';
        throw;
    }
}

} // namespace vhdl_fmt
