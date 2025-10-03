#include "Config.hpp"
#include "ConfigManager.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <span>
#include <string>
#include <vector>

/// The main entry point of the program
auto main(int argc, char *argv[]) -> int
{
    try {
        // Create configuration manager with command line arguments
        vhdl_fmt::ConfigManager configManager{ std::span<char *>(argv,
                                                                 static_cast<std::size_t>(argc)) };

        const auto &cliArgs = configManager.getCliArgs();

        // Handle version flag
        if (cliArgs.show_version) {
            std::cout << vhdl_fmt::ConfigReader::getVersion() << std::endl;
            return EXIT_SUCCESS;
        }

        // Handle help flag (argparse handles this automatically, but we check for consistency)
        if (cliArgs.show_help) {
            // Help is already printed by argparse
            return EXIT_SUCCESS;
        }

        // Get the final configuration
        const auto &config = configManager.getConfig();

        // Validate that we have input files to process
        if (cliArgs.input_files.empty()) {
            std::cerr << "Error: No input files specified." << std::endl;
            std::cerr << "Use --help for usage information." << std::endl;
            return EXIT_FAILURE;
        }

        // Display configuration summary (for debugging/verbose mode)
        std::cout << "VHDL Formatter Configuration:" << std::endl;
        std::cout << "  Line length: " << config.line_length << std::endl;
        std::cout
          << "  Indentation: "
          << (config.indentation.style == vhdl_fmt::IndentationStyle::SPACES ? "spaces" : "tabs")
          << " (size: "
          << config.indentation.size
          << ")"
          << std::endl;

        // Process input files
        std::cout << "Processing " << cliArgs.input_files.size() << " file(s):" << std::endl;
        for (const auto &file : cliArgs.input_files) {
            std::cout << "  - " << file << std::endl;

            // TODO: Call formatter with the configuration and file
            // This is where the actual formatting would happen
            // formatter.format(file, config);
        }

        std::cout << "Formatting completed successfully." << std::endl;
        return EXIT_SUCCESS;

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return EXIT_FAILURE;
    }
}
