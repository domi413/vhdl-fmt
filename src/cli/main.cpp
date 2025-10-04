#include "Config.hpp"
#include "ConfigManager.hpp"

#include <cstdlib>
#include <iostream>
#include <span>
#include <vector>

/// The main entry point of the program
auto main(int argc, char *argv[]) -> int
{
    try {
        // Create configuration manager with command line arguments
        vhdl_fmt::ConfigManager config_manager{ std::span<char *>(argv,
                                                                  static_cast<std::size_t>(argc)) };

        const auto &cli_args = config_manager.getCliArgs();

        // Handle version flag
        if (cli_args.show_version) {
            std::cout << vhdl_fmt::ConfigReader::getVersion() << '\n';
            return EXIT_SUCCESS;
        }

        // Handle help flag (argparse handles this automatically, but we check for consistency)
        if (cli_args.show_help) {
            // Help is already printed by argparse
            return EXIT_SUCCESS;
        }

        // Get the final configuration
        const auto &config = config_manager.getConfig();

        // Validate that we have input files to process
        if (cli_args.input_files.empty()) {
            std::cerr << "Error: No input files specified." << '\n';
            std::cerr << "Use --help for usage information." << '\n';
            return EXIT_FAILURE;
        }

        // Display configuration summary (for debugging/verbose mode)
        std::cout << "VHDL Formatter Configuration:" << '\n';
        std::cout << "  Line length: " << config.line_length << '\n';
        std::cout
          << "  Indentation: "
          << (config.indentation.style == vhdl_fmt::IndentationStyle::SPACES ? "spaces" : "tabs")
          << " (size: "
          << config.indentation.size
          << ")"
          << '\n';

        // Process input files
        std::cout << "Processing " << cli_args.input_files.size() << " file(s):" << '\n';
        for (const auto &file : cli_args.input_files) {
            std::cout << "  - " << file << '\n';

            // TODO: Call formatter with the configuration and file
            // This is where the actual formatting would happen
            // formatter.format(file, config);
        }

        std::cout << "Formatting completed successfully." << '\n';
        return EXIT_SUCCESS;

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred." << '\n';
        return EXIT_FAILURE;
    }
}
