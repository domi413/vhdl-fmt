#include "argument_parser.hpp"

// #include "version.hpp"

#include <argparse/argparse.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace cli {

ArgumentParser::ArgumentParser(std::span<char *> args)
{
    this->parseArguments(args);
}

auto ArgumentParser::getConfigPath() const noexcept -> const std::optional<std::filesystem::path> &
{
    return config_file_path_;
}

auto ArgumentParser::getFlags() const noexcept -> const std::unordered_map<std::string_view, bool> &
{
    return used_flags_;
}

auto ArgumentParser::parseArguments(std::span<char *> args) -> void
{
    // TODO(domi): After the `.in` file gets correctly generated, use PROJECT_NAME & PROJECT_VERSION
    argparse::ArgumentParser program{ "vhdl-fmt", "1.0.0" };

    program.add_description("A VHDL formatter for beautifying and standardizing VHDL code.");
    program.add_epilog("For more information, visit the project "
                       "documentation.\nhttps://github.com/domi413/vhdl-fmt");

    program.add_argument("input").help("VHDL file or directory to format").metavar("file.vhd");

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
      .action([this](const std::string &location) -> std::string {
          const std::filesystem::path config_path{ location };

          if (!std::filesystem::exists(config_path)) {
              throw std::runtime_error("Configuration file does not exist: " + location);
          }

          if (!std::filesystem::is_regular_file(config_path)) {
              throw std::runtime_error("Configuration path is not a regular file: " + location);
          }

          config_file_path_ = std::filesystem::canonical(config_path);
          return location;
      });

    try {
        std::vector<std::string> c_args;
        c_args.reserve(args.size());

        // NOTE: This loop is required because `parse_args()` expects a c-style array.
        for (const auto *arg : args) {
            if (arg != nullptr) {
                c_args.emplace_back(arg);
            }
        }

        program.parse_args(c_args);

        if (program.is_used("--write")) {
            used_flags_.at("write") = true;
        }
        if (program.is_used("--check")) {
            used_flags_.at("check") = true;
        }

    } catch (const std::exception &err) {
        std::cerr << "Error parsing arguments: " << err.what() << '\n';
        std::cerr << program << '\n';
        throw;
    }
}

} // namespace cli
